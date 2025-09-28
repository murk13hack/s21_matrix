# =============================================================================
# Compiler Configuration
# =============================================================================
CC				::=		gcc
CFLAGS			::=		-Wall -Werror -Wextra -std=c11 -pedantic -I./include -lm
TST_FLAG		::=		$(shell pkg-config --cflags --libs check)
COV_FLAGS		::=		-fprofile-arcs -ftest-coverage
DBG_FLAGS		::=		-g
REL_FLAG		::=		-DNDEBUG -O2