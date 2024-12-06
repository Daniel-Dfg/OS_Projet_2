CC = g++
CFLAGS = -Wall -Wextra

# Changez ce nom selon le scénario à compiler !
CASE_DIR = case2
OBJ_DIR = obj

# Trouver tous les fichiers source .cpp dans le dossier spécifié
SOURCES := $(wildcard $(CASE_DIR)/*.cpp)
# Générer les noms des exécutables dans le dossier obj
EXECUTABLES := $(SOURCES:$(CASE_DIR)/%.cpp=$(OBJ_DIR)/%)

# Créer les dossiers si nécessaire
$(shell mkdir -p $(OBJ_DIR))

all: $(EXECUTABLES)

# Règle générique pour compiler chaque fichier source
$(OBJ_DIR)/%: $(CASE_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(OBJ_DIR)

# Pour éviter les conflits avec des fichiers qui pourraient s'appeler 'clean'
.PHONY: clean all
