/*
 * Querying and printing the variables names of all available control variables
 */

#include "lib_utils.h"

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   int i, err, num, namelen, bind, verbose, read_only;
   int threadsupport;
   char name[100];
   MPI_Datatype datatype;
   int reqthread = MPI_THREAD_MULTIPLE;

   int rank;

   err = MPI_Init_thread(&argc, &argv, reqthread, &threadsupport);
   err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   if (rank == 0) {
      err = MPI_T_init_thread(reqthread, &threadsupport);
      if (err != MPI_SUCCESS)
         return err;
      err = MPI_T_pvar_get_num(&num);
      if (err != MPI_SUCCESS)
         return err;
      for (i = 0; i < num; i++) {
         namelen = 100;

         int varclass, continuous, atomic;
         char *desc = NULL;
         int desc_len;
         err = MPI_T_pvar_get_info(i, name, &namelen, &verbose, &varclass,
                                   &datatype, NULL, desc, &desc_len, &bind,
                                   &read_only, &continuous, &atomic);

         if (err != MPI_SUCCESS && err != MPI_T_ERR_INVALID_INDEX)
            return err;
         printf("PVar %i\n"
                "- Name       : %s\n"
                "- Verbosity  : %d => %s\n"
                "- Varclass   : %d => %s\n"
                "- Datatype   : %d => %s\n"
                "- Desc       : %s\n"
                "- Bind       : %d => %s\n"
                "- Read-only  : %d\n"
                "- Continuous : %d\n"
                "- Atomic     : %d\n",
                i, name, verbose, get_verbose(verbose), varclass,
                get_varclass(varclass), datatype, get_datatype(datatype),
                desc ? desc : "null", bind, get_bind(bind), read_only,
                continuous, atomic);
      }
      return MPI_T_finalize();
   }
   MPI_Barrier(MPI_COMM_WORLD);
   MPI_Finalize();
}