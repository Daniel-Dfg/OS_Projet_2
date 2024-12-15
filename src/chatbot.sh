#!/usr/bin/env bash

set -e  # Arrête le script dès un "exit"

# Vérifie les pseudonymes, laisse car seras utile pour vérifier si le nouveau pseudo est correcte
function verify_pseudo() {
    local user_name="$1"
    local bot_name="$2"
    local max_length=6

    # Vérifie si les pseudonymes sont identiques
    if [[ "$user_name" == "$bot_name" ]]; then
        echo "Erreur : Vous ne pouvez pas avoir le même nom que votre destinataire :( " >&2
        return 1
    fi

    # Vérifie la longueur des pseudonymes
    if (( ${#user_name} > max_length || ${#bot_name} > max_length )); then
        echo "Erreur : Longueur du pseudo utilisateur/destinataire doit être inférieure ou égale à $max_length caractères" >&2
        return 1
    fi

    return 0
}

# Vérifie si les entrées sont correctes
function verify_entry() {
    local num_args="$#"
    local user_name=""
    local bot_name="bot"  # Valeur par défaut

    # Vérifie le nombre d'arguments
    if [[ $num_args -ne 1 && $num_args -ne 2 ]]; then
        echo "chat-bot utilisateur destinataire" >&2
        exit 1
    fi

    user_name="$1"
    if [[ $num_args -eq 2 ]]; then
        bot_name="$2"
    fi

    # Vérifie les pseudonymes
    if ! verify_pseudo "$user_name" "$bot_name"; then
        exit 1
    fi

    # Retourne les pseudonymes valides
    echo "$user_name" "$bot_name"
}

# Fonction principale de communication avec le bot
function chat() {
    local user_name="$1"
    local receiver_name="$2"

    # Lancement du processus bot
    coproc BOT_PROCESS { ./chat "$user_name" --bot; }

    exec {input_to_bot}>&"${BOT_PROCESS[1]}"
    exec {output_from_bot}<&"${BOT_PROCESS[0]}"


    echo "Le destinataire est : $receiver_name"
    echo "Appuyez sur Ctrl+D pour changer le destinataire."


    while true; do
        # envoie le message de l'utilisateur au destinataire
        if read -t 0.1 -r message; then
            echo "$receiver_name $message" >&$input_to_bot
        fi

        # recupère les message du destinataire
        if read -t 0.1 -r bot_message <&$output_from_bot; then
            echo "$bot_message"

            #Vider les messages qui sont en trop dans le buffer ? un truc du genre  qui faudras faire

            #while read -r bot_message <&$output_from_bot; do
            #    echo "$bot_message"
            #done

        fi
    done


    # Fermeture des descripteurs
    exec {input_to_bot}>&-
    exec {output_from_bot}<&-
}

# Fonction principale
function main() {
    local user_name bot_name
    # Lis l'entrée utilisateur
    read -r user_name bot_name < <(verify_entry "$@")
    chat "$user_name" "$bot_name"
}

main "$@"

# mettre un executif de ./chat dans le même repertoire que chatbot.sh, faire chmod -x chatbot.sh , ./chatbot.sh alice bob

# Le problème va venir du "read":

# (1) Si il y a plus de 64 messages de 1024 octet envoyé par 0.1, le pipe seras déborder et il y auras une erreur.
# (2) Si il y a plusieurs message qui vienne durant la 0.1 seconde, le read ne pourras lire que un message à chaque itération de la boucle while(true)
# et les messages non lus sur la pipe resterons présent tant qu'il ne sont pas traité
# Si on veux géré ces expection là, on va devoir utiliser des mécanismes alternatif, peut etre select?

# TODO : 
# gestion du CTRL + D
# Verifiez si ça marche avec mode --manuel
# Trouvez une solution alternative à read?
# Faire la gestion des erreurs
# il faudras sûrement un mécanisme comme select