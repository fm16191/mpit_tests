/*
 * Querying and printing the variables names of all available control variables
 */

#include "lib_utils.h"

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   int i, err, num;
   int verbose, bind, scope;
   MPI_T_enum enumtype;
   int threadsupport;

   MPI_Datatype datatype;

   err = MPI_T_init_thread(MPI_THREAD_SINGLE, &threadsupport);
   if (err != MPI_SUCCESS)
      return err;
   err = MPI_T_cvar_get_num(&num);
   if (err != MPI_SUCCESS)
      return err;

   printf("Number of control variables : %d\n", num);

   for (i = 0; i < num; i++) {
      char *name = NULL, *desc = NULL;
      int name_len = 0, desc_len = 0;
      int err = MPI_T_cvar_get_info(i, name, &name_len, NULL, NULL, NULL, desc,
                                    &desc_len, NULL, NULL);

      name = malloc(sizeof(char) * name_len);
      desc = malloc(sizeof(char) * desc_len);

      err = MPI_T_cvar_get_info(i, name, &name_len, &verbose, &datatype,
                                &enumtype, desc, &desc_len, &bind, &scope);
      if (err != MPI_SUCCESS && err != MPI_T_ERR_INVALID_INDEX)
         return err;

      /* Read cvar value */
      MPI_T_cvar_handle handle = MPI_T_CVAR_HANDLE_NULL;
      int count;
      int v_int;
      unsigned int v_uint;
      unsigned long v_ulong;
      unsigned long long v_ullong;
      MPI_Count v_count;
      char v_char[4097];
      double v_double;
      char value[257];

      if (bind == MPI_T_BIND_NO_OBJECT) {
         err = MPI_T_cvar_handle_alloc(i, NULL, &handle, &count);
         assert(err == MPI_SUCCESS);
      }
      else if (bind == MPI_T_BIND_MPI_COMM) {
         MPI_Comm comm = MPI_COMM_WORLD;
         err = MPI_T_cvar_handle_alloc(i, &comm, &handle, &count);
         assert(err == MPI_SUCCESS);
      }
      else {
         sprintf(value, "unsupported");
      }

      if (count == 1 || datatype == MPI_CHAR) {
         if (datatype == MPI_INT) {
            err = MPI_T_cvar_read(handle, &v_int);

            if (enumtype == MPI_T_ENUM_NULL) {
               sprintf(value, "%i", v_int);
            }
            else {
               int i, etnum;
               char etname[20];
               int etlen = 20;
               int done = 0;
               int newval;
               err = MPI_T_enum_get_info(enumtype, &etnum, etname, &etlen);
               for (i = 0; i < etnum; i++) {
                  etlen = 12;
                  err =
                     MPI_T_enum_get_item(enumtype, i, &newval, etname, &etlen);
                  if (newval == v_int) {
                     sprintf(value, "%s", etname);
                     done = 1;
                  }
               }
               if (!done) {
                  sprintf(value, "unknown");
               }
            }
         }
         else if (datatype == MPI_UNSIGNED) {
            err = MPI_T_cvar_read(handle, &v_uint);
            assert(err == MPI_SUCCESS);
            sprintf(value, "%u", v_uint);
         }
         else if (datatype == MPI_UNSIGNED_LONG) {
            err = MPI_T_cvar_read(handle, &v_ulong);
            assert(err == MPI_SUCCESS);
            sprintf(value, "%lu", v_ulong);
         }
         else if (datatype == MPI_UNSIGNED_LONG_LONG) {
            err = MPI_T_cvar_read(handle, &v_ullong);
            assert(err == MPI_SUCCESS);
            sprintf(value, "%llu", v_ullong);
         }
         else if (datatype == MPI_COUNT) {
            err = MPI_T_cvar_read(handle, &v_count);
            assert(err == MPI_SUCCESS);
            sprintf(value, "%llu", v_count);
         }
         else if (datatype == MPI_CHAR) {
            err = MPI_T_cvar_read(handle, v_char);
            assert(err == MPI_SUCCESS);
            // sprintf(value, "%s", v_char);
            strcpy(value, v_char);
         }
         else if (datatype == MPI_DOUBLE) {
            err = MPI_T_cvar_read(handle, &v_double);
            assert(err == MPI_SUCCESS);
            sprintf(value, "%f", v_double);
         }
         else {
            sprintf(value, "unsupported");
         }
      }
      else {
         sprintf(value, "unsupported");
      }

      printf("CVar %i\n"
             "- Name      : %s\n"
             "- Verbosity : %d => %s\n"
             "- Datatype  : %d => %s\n"
             "- Desc      : %s\n"
             "- Bind      : %d => %s\n"
             "- Scope     : %d => %s\n"
             "- Value     : %s\n",
             i, name, verbose, get_verbose(verbose), datatype,
             get_datatype(datatype), desc ? desc : "", bind, get_bind(bind),
             scope, get_scope(scope), value);

      free(name);
      free(desc);
   }

   return MPI_T_finalize();
}