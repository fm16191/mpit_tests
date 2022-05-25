#pragma once

#define RED "\033[31m"
#define LIGHTRED "\033[91m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define LIGHTCYAN "\033[96m"
#define BOLD "\033[1m"
#define NORMAL "\033[0m"
#define DIM "\033[2m"

#define red(msg) fprintf(stderr, "%s%sError: %s %s\n", BOLD, RED, NORMAL, msg);
#define green(msg) fprintf(stderr, "%s%s%s %s\n", BOLD, GREEN, NORMAL, msg);