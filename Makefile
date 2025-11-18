# ==================================
# 1. Configuration Générale
# ==================================
.DEFAULT_GOAL := default
EXEC := template
VERSION := 1.0

# Dossiers principaux
SRC_DIR := src
INC_DIR := include
DIST_DIR := dist
LOG_DIR := logs

# ==================================
# 2. Configuration Cible & Build
# ==================================
KNOWN_TARGETS := \
	linux-x86_64 \
	linux-i386 \
	windows-x86_64 \
	windows-i386

# Détection Hôte
ifeq ($(OS),Windows_NT)
	HOST_PLATFORM := windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
	   HOST_PLATFORM := linux
	else ifeq ($(UNAME_S),Darwin)
	   HOST_PLATFORM := macos
	else
	   $(error Unsupported host OS: $(UNAME_S))
	endif
endif

UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),x86_64)
	HOST_ARCH := x86_64
else ifeq ($(UNAME_M),i686)
	HOST_ARCH := i386
else ifeq ($(UNAME_M),i386)
	HOST_ARCH := i386
else ifeq ($(UNAME_M),aarch64)
	HOST_ARCH := arm64
else
	$(error Unknown host architecture: $(UNAME_M))
endif

HOST_TARGET := $(HOST_PLATFORM)-$(HOST_ARCH)

# Sélection Cible & Build
TARGET ?= $(HOST_TARGET)
BUILD ?= release

PLATFORM := $(firstword $(subst -, ,$(TARGET)))
ARCH := $(lastword $(subst -, ,$(TARGET)))

# Définition des Dossiers Dynamiques
OBJ_DIR := build/$(BUILD)/$(TARGET)
BIN_DIR := bin/$(BUILD)/$(TARGET)
LIB_DIR := lib/$(TARGET)

# ==================================
# 3. Toolchain & Utilitaires
# ==================================
VERBOSE ?= 0
ifeq ($(VERBOSE), 1)
	HIDE :=
else
	HIDE := @
endif

CC  ?= gcc
CXX ?= g++
MKDIR := mkdir -p
RM    := rm -rf

# ==================================
# 4. Flags de Compilation
# ==================================
# Flags de base
CFLAGS   := -std=c11
CXXFLAGS := -std=c++17
CPPFLAGS := -Wall -Wextra -I$(INC_DIR)
LDFLAGS  := -L$(LIB_DIR)

# Suffixe Exécutable
EXEC_EXT :=

# Flags Spécifiques à la Cible
ifeq ($(PLATFORM),windows)
	EXEC_EXT := .exe
	ifeq ($(ARCH),i386)
	   CC  := i686-w64-mingw32-gcc
	   CXX := i686-w64-mingw32-g++
	   CPPFLAGS += -m32
	   LDFLAGS  += -m32 -Wl,--dynamicbase -Wl,--nxcompat
	else ifeq ($(ARCH),x86_64)
	   CC  := x86_64-w64-mingw32-gcc
	   CXX := x86_64-w64-mingw32-g++
	   LDFLAGS  += -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va
	else
	   $(error Unsupported architecture $(ARCH) for $(PLATFORM))
	endif

	LDFLAGS  += -lmingw32 -lSDL2main -lSDL2 -mwindows \
			 -lSDL2_image -lSDL2_mixer -lm \
			 -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 \
			 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 \
			 -lsetupapi -lversion -luuid

else ifeq ($(PLATFORM),linux)
	ifeq ($(ARCH),i386)
	   CPPFLAGS += -m32
	   LDFLAGS  += -m32
	else ifeq ($(ARCH),x86_64)
	else ifeq ($(ARCH),arm64)
	   $(error ARM target (linux-arm64) is not yet implemented.)
	else
	   $(error Unsupported architecture $(ARCH) for $(PLATFORM))
	endif
	CPPFLAGS += -D_REENTRANT
	LDFLAGS  += -Wl,--enable-new-dtags -lSDL2 -lSDL2_image -lSDL2_mixer -pthread -lm

else ifeq ($(PLATFORM),macos)
	$(error macOS target is not yet implemented.)
else
	$(error Unsupported target: $(TARGET))
endif

# Flags Spécifiques au Type de Build
HARDEN_FLAGS := -fstack-protector-strong -fstack-clash-protection \
			 -fcf-protection -Wformat -Werror=format-security

ifeq ($(BUILD),debug)
	CFLAGS   += -g -Og
	CXXFLAGS += -g -Og
	CPPFLAGS += -D_DEBUG
else ifeq ($(BUILD),release)
	CFLAGS   += -O2 $(HARDEN_FLAGS)
	CXXFLAGS += -O2 $(HARDEN_FLAGS)
	CPPFLAGS += -flto -D_FORTIFY_SOURCE=2 -Wdate-time
	LDFLAGS  += -flto -s
	ifneq ($(PLATFORM),windows)
	   LDFLAGS += -Wl,-z,relro
	endif
endif

# ==================================
# 5. Génération des Fichiers
# ==================================
SRCS := $(shell find $(SRC_DIR) -type f \( -name "*.c" -o -name "*.cpp" \))
OBJS := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SRCS:.c=.o))
OBJS := $(OBJS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

# ==================================
# 6. Règles de Build Principales
# ==================================
.PHONY: default all run clean fclean re distclean help package valgrind

# Cible par défaut : construit le TARGET et BUILD actuel
default: $(BIN_DIR)/$(EXEC)$(EXEC_EXT)

# Cible 'all' : construit TOUTES les cibles, en mode release
all:
	@echo "--- Building all known targets (release) ---"
	@$(foreach t,$(KNOWN_TARGETS),$(MAKE) -rR default TARGET=$(t) BUILD=release || exit 1;)
	@echo "--- All targets built successfully ---"

# Règle de Linkage
$(BIN_DIR)/$(EXEC)$(EXEC_EXT): $(OBJS)
	$(HIDE)@$(MKDIR) $(BIN_DIR)
	$(HIDE)@echo " [LD] $(TARGET) [$(BUILD)] -> $@"
	$(HIDE)$(CXX) -o $@ $^ $(LDFLAGS)

# Règle de compilation C
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(HIDE)@$(MKDIR) $(dir $@)
	$(HIDE)@echo " [CC] $(TARGET) [$(BUILD)] <- $<"
	$(HIDE)$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

# Règle de compilation C++
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(HIDE)@$(MKDIR) $(dir $@)
	$(HIDE)@echo " [C++] $(TARGET) [$(BUILD)] <- $<"
	$(HIDE)$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

# Inclusion des dépendances
-include $(DEPS)

# ==================================
# 7. Règles de Test & Analyse
# ==================================
run: default
	@echo " [RUN] Lancement de $(TARGET) [$(BUILD)]..."
	$(BIN_DIR)/$(EXEC)$(EXEC_EXT)

VALGRIND_FLAGS ?= --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file="$(LOG_DIR)/valgrind-%p.log"

valgrind:
	@if [ "$(HOST_PLATFORM)" != "linux" ]; then \
		echo " [ERROR] La cible 'valgrind' n'est supportée que sur Linux."; \
		exit 1; \
	fi
	@echo " [PREP] Compilation en mode 'debug' pour Valgrind (cible $(HOST_TARGET))..."
	@$(MAKE) -rR default TARGET=$(HOST_TARGET) BUILD=debug
	@echo " [PREP] Création du dossier $(LOG_DIR)..."
	@$(MKDIR) -p $(LOG_DIR)
	@echo " [VALGRIND] Lancement de Valgrind... Logs dans $(LOG_DIR)/"
	@valgrind $(VALGRIND_FLAGS) ./bin/debug/$(HOST_TARGET)/$(EXEC)
	@echo " [VALGRIND] Exécution terminée."

# ==================================
# 8. Règles de Distribution
# ==================================
.PHONY: package package-clean
package: $(addprefix package-,$(KNOWN_TARGETS))
	@echo "--- Tous les packages ont été créés dans $(DIST_DIR)/ ---"

$(addprefix package-,$(KNOWN_TARGETS)): | package-clean

package-clean:
	@echo " [CLEAN] Nettoyage du dossier $(DIST_DIR)"
	$(HIDE)$(RM) -r $(DIST_DIR)
	$(HIDE)$(MKDIR) -p $(DIST_DIR)

package-%:
	@echo " [PACK] Traitement de la cible : $*"
	$(HIDE)$(MAKE) -rR default TARGET=$* BUILD=release
	$(HIDE) \
	PACKAGE_NAME="$(EXEC)-$(VERSION)-$*"; \
	PACKAGE_DIR="$(DIST_DIR)/$$PACKAGE_NAME"; \
	SOURCE_BIN_DIR="bin/release/$*"; \
	PLATFORM_NAME=$$(echo "$*" | cut -d- -f1); \
	\
	if [ "$$PLATFORM_NAME" = "windows" ]; then \
		EXEC_NAME="$(EXEC).exe"; \
	else \
		EXEC_NAME="$(EXEC)"; \
	fi; \
	\
	echo " [PREP] Préparation du dossier $$PACKAGE_DIR"; \
	$(MKDIR) -p "$$PACKAGE_DIR"; \
	cp "$$SOURCE_BIN_DIR/$$EXEC_NAME" "$$PACKAGE_DIR/"; \
	if [ -f README.md ]; then cp README.md "$$PACKAGE_DIR/"; fi; \
	\
	echo " [ZIP] Création de $(DIST_DIR)/$$PACKAGE_NAME.zip"; \
	cd $(DIST_DIR) && zip -r -q "$$PACKAGE_NAME.zip" "$$PACKAGE_NAME"; \
	\
	echo " [CLEAN] Nettoyage du dossier temporaire $$PACKAGE_DIR"; \
	$(RM) -r "$$PACKAGE_DIR"; \
	echo " [DONE] Package pour $* créé."

# ==================================
# 9. Règles de Nettoyage
# ==================================
clean:
	@echo " [CLEAN] $(OBJ_DIR)"
	$(HIDE)$(RM) -r $(OBJ_DIR)

fclean: clean
	@echo " [FCLEAN] $(BIN_DIR)"
	$(HIDE)$(RM) -r $(BIN_DIR)

re: fclean default

distclean:
	@echo " [DISTCLEAN] Removing all 'build', 'bin' and 'dist' directories"
	$(HIDE)$(RM) -r build bin $(DIST_DIR)

# ==================================
# 10. Cible d'aide
# ==================================
help:
	@echo "Usage:  make <commande> [VARIABLE=valeur]"
	@echo ""
	@echo "Commandes principales:"
	@echo "  default          Compiler le projet (BUILD=$(BUILD), TARGET=$(TARGET))."
	@echo "  all              Compiler toutes les cibles connues (en mode 'release')."
	@echo "  run              Compiler et exécuter le projet (BUILD=$(BUILD), TARGET=$(TARGET))."
	@echo "  valgrind         Exécuter le projet (Linux) avec Valgrind (force BUILD=debug)."
	@echo "  package          Créer des .zip de distribution (BUILD=release) pour chaque cible."
	@echo "  clean            Nettoyer les fichiers objets du build actuel."
	@echo "  fclean           Nettoyer les objets ET l'exécutable du build actuel."
	@echo "  re               Recompiler (fclean + default)."
	@echo "  distclean        Nettoyer TOUS les dossiers 'build', 'bin' et 'dist'."
	@echo "  help             Afficher ce message d'aide."
	@echo ""
	@echo "Variables de configuration:"
	@echo "  TARGET=<cible>"
	@echo "    Spécifie la cible de build (plateforme-architecture)."
	@echo "    Défaut auto-détecté: $(HOST_TARGET)"
	@echo "    Cibles connues: $(KNOWN_TARGETS)"
	@echo ""
	@echo "  BUILD=<type>"
	@echo "    Spécifie le type de build (debug ou release)."
	@echo "    Défaut: $(BUILD)"
	@echo "    Valeurs:  release, debug"
	@echo ""
	@echo "  VERSION=<ver>"
	@echo "    Spécifie la version pour 'make package' (Défaut: $(VERSION))"
	@echo ""
	@echo "  VERBOSE=1"
	@echo "    Afficher les commandes de compilation complètes."
	@echo ""
	@echo "  VALGRIND_FLAGS=..."
	@echo "    Spécifie les flags pour 'make valgrind' (Défaut: --leak-check=full...)"
