/*
 * Querying and printing the variables names of all available control variables
 */

#include "mpit_utils.h"

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   int i, err, num;
   int verbose, varclass, bind, read_only, continuous, atomic;
   int threadsupport;
   MPI_Datatype datatype;
   MPI_T_enum enumtype;

   int rank;

   // err = MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &threadsupport);
   err = MPI_Init(&argc, &argv);
   assert(err == MPI_SUCCESS);
   err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   assert(err == MPI_SUCCESS);

   err = MPI_T_init_thread(MPI_THREAD_MULTIPLE, &threadsupport);
   if (err != MPI_SUCCESS)
      return err;

   /* Start pvar session */
   MPI_T_pvar_session session;
   err = MPI_T_pvar_session_create(&session);
   assert(err == MPI_SUCCESS);

   /* Get total pvar count */
   err = MPI_T_pvar_get_num(&num);
   if (err != MPI_SUCCESS)
      return err;

   static MPI_T_pvar_handle pvar_handle = MPI_T_PVAR_HANDLE_NULL;

   for (i = 0; i < num; i++) {
      char *name = NULL, *desc = NULL;
      int desc_len = 0, name_len = 0;
      err = MPI_T_pvar_get_info(i, NULL, &name_len, NULL, NULL, NULL, NULL,
                                NULL, &desc_len, NULL, NULL, NULL, NULL);
      assert(err == MPI_SUCCESS);

      name = malloc(sizeof(char) * name_len);
      desc = malloc(sizeof(char) * desc_len);

      err = MPI_T_pvar_get_info(i, name, &name_len, &verbose, &varclass,
                                &datatype, &enumtype, desc, &desc_len, &bind,
                                &read_only, &continuous, &atomic);
      assert(err == MPI_SUCCESS);

      /* Read pvar value */
      int count = 0;
      int v_int = 0;
      unsigned int v_uint = 0;
      unsigned long v_ulong = 0;
      unsigned long long v_ullong = 0;
      MPI_Count v_count = 0;
      char v_char[4097];
      double v_double = 0;
      char value[257];

      if (bind == MPI_T_BIND_NO_OBJECT) {
         err = MPI_T_pvar_handle_alloc(session, i, NULL, &pvar_handle, &count);
         assert(pvar_handle != MPI_T_PVAR_HANDLE_NULL);
      }
      else if (bind == MPI_T_BIND_MPI_COMM) {
         MPI_Comm comm = MPI_COMM_WORLD;
         err = MPI_T_pvar_handle_alloc(session, i, &comm, &pvar_handle, &count);
         assert(pvar_handle != MPI_T_PVAR_HANDLE_NULL);
      }
      else {
         sprintf(value, "unsupported");
      }

      if (count == 1 || datatype == MPI_CHAR) {
         if (datatype == MPI_INT) {
            err = MPI_T_pvar_read(session, pvar_handle, &v_int);

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
            err = MPI_T_pvar_read(session, pvar_handle, &v_uint);
            sprintf(value, "%u", v_uint);
         }
         else if (datatype == MPI_UNSIGNED_LONG) {
            err = MPI_T_pvar_read(session, pvar_handle, &v_ulong);
            sprintf(value, "%lu", v_ulong);
         }
         else if (datatype == MPI_UNSIGNED_LONG_LONG) {
            err = MPI_T_pvar_read(session, pvar_handle, &v_ullong);
            sprintf(value, "%llu", v_ullong);
         }
         else if (datatype == MPI_COUNT) {
            err = MPI_T_pvar_read(session, pvar_handle, &v_count);
            sprintf(value, "%llu", v_count);
         }
         else if (datatype == MPI_CHAR) {
            err = MPI_T_pvar_read(session, pvar_handle, v_char);
            strcpy(value, v_char);
         }
         else if (datatype == MPI_DOUBLE) {
            err = MPI_T_pvar_read(session, pvar_handle, &v_double);
            sprintf(value, "%f", v_double);
         }
         else {
            sprintf(value, "unsupported");
         }
      }
      else {
         sprintf(value, "unsupported");
      }

      /* Print pvar */
      printf("PVar %i\n"
             "- Name       : %s\n"
             "- Verbosity  : %d => %s\n"
             "- Varclass   : %d => %s\n"
             "- Datatype   : %d => %s\n"
             "- Desc       : %s\n"
             "- Bind       : %d => %s\n"
             "- Read-only  : %d\n"
             "- Continuous : %d\n"
             "- Atomic     : %d\n"
             "- Count      : %d\n"
             "- Value      : %s\n",
             i, name, verbose, get_verbose(verbose), varclass,
             get_varclass(varclass), datatype, get_datatype(datatype),
             desc ? desc : "null", bind, get_bind(bind), read_only, continuous,
             atomic, count, value);
   }

   MPI_T_pvar_handle_free(session, &pvar_handle);
   err = MPI_T_pvar_session_free(&session);

   return MPI_T_finalize();

   MPI_Barrier(MPI_COMM_WORLD);
   MPI_Finalize();
}