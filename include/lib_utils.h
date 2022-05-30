#include <assert.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_safety(MPI_T_cb_safety safety)
{
   switch (safety) {
      case MPI_T_CB_REQUIRE_NONE:
         return "MPI_T_CB_REQUIRE_NONE ";
         break;
      case MPI_T_CB_REQUIRE_MPI_RESTRICTED:
         return "MPI_T_CB_REQUIRE_MPI_RESTRICTED ";
         break;
      case MPI_T_CB_REQUIRE_THREAD_SAFE:
         return "MPI_T_CB_REQUIRE_THREAD_SAFE ";
         break;
      case MPI_T_CB_REQUIRE_ASYNC_SIGNAL_SAFE:
         return "MPI_T_CB_REQUIRE_ASYNC_SIGNAL_SAFE ";
         break;
      default:
         return "";
         break;
   }
}

char *get_type(MPI_Datatype type)
{
   switch (type) {
      case MPI_INT:
         return "INT   ";
         break;
      case MPI_UNSIGNED:
         return "UINT  ";
         break;
      case MPI_UNSIGNED_LONG:
         return "ULONG  ";
         break;
      case MPI_UNSIGNED_LONG_LONG:
         return "ULLONG ";
         break;
      case MPI_COUNT:
         return "COUNT  ";
         break;
      case MPI_CHAR:
         return "CHAR  ";
         break;
      case MPI_DOUBLE:
         return "DOUBLE";
         break;
      default:
         return "";
         break;
   }
}

char *get_datatype(MPI_Datatype datatype)
{
   switch (datatype) {
      case MPI_CHAR: // 0x4c000101:
         return "MPI_CHAR";
      case MPI_SIGNED_CHAR: // 0x4c000118:
         return "MPI_SIGNED_CHAR";
      case MPI_UNSIGNED_CHAR: // 0x4c000102:
         return "MPI_UNSIGNED_CHAR";
      case MPI_BYTE: // 0x4c00010d:
         return "MPI_BYTE";
      case MPI_WCHAR: // 0x4c00040e:
         return "MPI_WCHAR";
      case MPI_SHORT: // 0x4c000203:
         return "MPI_SHORT";
      case MPI_UNSIGNED_SHORT: // 0x4c000204:
         return "MPI_UNSIGNED_SHORT";
      case MPI_INT: // 0x4c000405:
         return "MPI_INT";
      case MPI_UNSIGNED: // 0x4c000406:
         return "MPI_UNSIGNED";
      case MPI_LONG: // 0x4c000807:
         return "MPI_LONG";
      case MPI_UNSIGNED_LONG: // 0x4c000808:
         return "MPI_UNSIGNED_LONG";
      case MPI_FLOAT: // 0x4c00040a:
         return "MPI_FLOAT";
      case MPI_DOUBLE: // 0x4c00080b:
         return "MPI_DOUBLE";
      case MPI_LONG_DOUBLE: // 0x4c00100c:
         return "MPI_LONG_DOUBLE";
      case MPI_LONG_LONG_INT: // 0x4c000809:
         return "MPI_LONG_LONG_INT";
      case MPI_UNSIGNED_LONG_LONG: // 0x4c000819:
         return "MPI_UNSIGNED_LONG_LONG";
      default:
         return "";
   }
}

char *get_value(MPI_Datatype datatype, void *buf)
{
   char *ret = NULL;
   char *type = get_datatype(datatype);
   // if (strcmp(type, "MPI_UNSIGNED") == 0)
   //    printf("%u", *(unsigned *)buf);
   // // sprintf(ret, "%u", (unsigned)buf);
   // else if (strcmp(type, "MPI_UNSIGNED_LONG_LONG") == 0)
   //    printf("%llu", *(unsigned long long *)buf);
   // sprintf(ret, "%llu", (unsigned long long)buf);
   if (strcmp(type, "MPI_DOUBLE") == 0) {
      double n = *(double *)buf;
      printf("%f\n", n);
      // printf("%f", (double)buf);
      // sprintf(ret, "%f", (double)buf);
   }
   return ret;
}

char *get_scope(int scope)
{
   switch (scope) {
      case MPI_T_SCOPE_CONSTANT:
         return "CONST";
         break;
      case MPI_T_SCOPE_READONLY:
         return "READONLY";
         break;
      case MPI_T_SCOPE_LOCAL:
         return "LOCAL";
         break;
      case MPI_T_SCOPE_GROUP:
         return "GROUP";
         break;
      case MPI_T_SCOPE_GROUP_EQ:
         return "GROUP-E";
         break;
      case MPI_T_SCOPE_ALL:
         return "ALL-EQ";
         break;
      case MPI_T_SCOPE_ALL_EQ:
         return "ALL";
         break;
      default:
         return "UNKNOWN";
         break;
   }
}

char *get_bind(int bind)
{
   switch (bind) {
      case MPI_T_BIND_NO_OBJECT:
         return "n/a     ";
         break;
      case MPI_T_BIND_MPI_COMM:
         return "COMM    ";
         break;
      case MPI_T_BIND_MPI_DATATYPE:
         return "DATATYPE";
         break;
      case MPI_T_BIND_MPI_ERRHANDLER:
         return "ERRHAND ";
         break;
      case MPI_T_BIND_MPI_FILE:
         return "FILE    ";
         break;
      case MPI_T_BIND_MPI_GROUP:
         return "GROUP   ";
         break;
      case MPI_T_BIND_MPI_OP:
         return "OP      ";
         break;
      case MPI_T_BIND_MPI_REQUEST:
         return "REQUEST ";
         break;
      case MPI_T_BIND_MPI_WIN:
         return "WINDOW  ";
         break;
      case MPI_T_BIND_MPI_MESSAGE:
         return "MESSAGE ";
         break;
      case MPI_T_BIND_MPI_INFO:
         return "INFO    ";
         break;
      default:
         return "UNKNOWN ";
         break;
   }
}

char *get_varclass(int varclass)
{
   switch (varclass) {
      case MPI_T_PVAR_CLASS_STATE:
         return "STATE  ";
         break;
      case MPI_T_PVAR_CLASS_LEVEL:
         return "LEVEL  ";
         break;
      case MPI_T_PVAR_CLASS_SIZE:
         return "SIZE   ";
         break;
      case MPI_T_PVAR_CLASS_PERCENTAGE:
         return "PERCENT";
         break;
      case MPI_T_PVAR_CLASS_HIGHWATERMARK:
         return "HIGHWAT";
         break;
      case MPI_T_PVAR_CLASS_LOWWATERMARK:
         return "LOWWAT ";
         break;
      case MPI_T_PVAR_CLASS_COUNTER:
         return "COUNTER";
         break;
      case MPI_T_PVAR_CLASS_AGGREGATE:
         return "AGGR   ";
         break;
      case MPI_T_PVAR_CLASS_TIMER:
         return "TIMER  ";
         break;
      case MPI_T_PVAR_CLASS_GENERIC:
         return "GENERIC";
         break;
      default:
         return "UNKNOWN";
         break;
   }
}

char *get_verbose(int verbose)
{
   switch (verbose) {
      case MPI_T_VERBOSITY_USER_BASIC:
         return "User/Basic";
         break;
      case MPI_T_VERBOSITY_USER_DETAIL:
         return "User/Detail";
         break;
      case MPI_T_VERBOSITY_USER_ALL:
         return "User/All";
         break;
      case MPI_T_VERBOSITY_TUNER_BASIC:
         return "Tuner/Basic";
         break;
      case MPI_T_VERBOSITY_TUNER_DETAIL:
         return "Tuner/Detail";
         break;
      case MPI_T_VERBOSITY_TUNER_ALL:
         return "Tuner/All";
         break;
      case MPI_T_VERBOSITY_MPIDEV_BASIC:
         return "Developer/Basic";
         break;
      case MPI_T_VERBOSITY_MPIDEV_DETAIL:
         return "Developer/Detail";
         break;
      case MPI_T_VERBOSITY_MPIDEV_ALL:
         return "Developer/All";
         break;
      default:
         return "UNKNOWN";
         break;
   }
}

char *get_thread_support(int threadsupport)
{
   switch (threadsupport) {
      case MPI_THREAD_SINGLE:
         return "MPI_THREAD_SINGLE\n";
         break;
      case MPI_THREAD_FUNNELED:
         return "MPI_THREAD_FUNNELED\n";
         break;
      case MPI_THREAD_SERIALIZED:
         return "MPI_THREAD_SERIALIZED\n";
         break;
      case MPI_THREAD_MULTIPLE:
         return "MPI_THREAD_MULTIPLE\n";
         break;
      default:
         return "";
   }
}

void print_info(MPI_Info info)
{
   infos_keys_t keys = get_info(info);

   printf("- Info keys : %d\n", keys.num);
   for (int i = 0; i < keys.num; i++)
      printf("  - Key   : %s\n"
             "  - Value : %s\n"
             "  - Flag  : %d\n",
             keys.key[i], keys.value[i] ? keys.value[i] : "null", keys.flag[i]);
}

typedef struct infos_keys_s {
   int num;
   char **key;
   char **value;
   int *flag;
} infos_keys_t;

free_info(infos_keys_t keys)
{
   free(keys.key);
   free(keys.value);
   free(keys.flag);
}

infos_keys_t get_info(MPI_Info info)
{
   infos_keys_t keys;
   int nkeys;
   if (info == MPI_INFO_NULL) {
      keys.num = 0;
      return keys;
   }
   MPI_Info_get_nkeys(info, &nkeys);
   keys.num = nkeys;
   keys.key = malloc(sizeof(char) * nkeys);
   keys.value = malloc(sizeof(char *) * nkeys);
   keys.flag = malloc(sizeof(int) * nkeys);

   for (int i = 0; i < nkeys; i++) {
      char key[MPI_MAX_INFO_KEY], *value = NULL;
      keys.key[i] = malloc(sizeof(char) * MPI_MAX_INFO_KEY);
      int value_len = 0, flag;

      MPI_Info_get_nthkey(info, i, key);
      MPI_Info_get(info, key, value_len, value, &flag);

      keys.key[i] = key;
      keys.value[i] = value;
      keys.flag[i] = flag;
   }
   return keys;
}