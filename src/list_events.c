#include "lib_utils.h"
#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>

static int verbose = 0;

int main(int argc, char *argv[])
{
   int rank;
   int required, provided;

   required = MPI_THREAD_SINGLE;
   MPI_T_init_thread(required, &provided);

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   int num_events;
   MPI_T_event_get_num(&num_events);

   printf("Num events : %d\n", num_events);

   int num_elements;
   MPI_Datatype *datatypes;
   MPI_Aint *displacements;
   MPI_T_enum enumtype;
   MPI_Info info;
   int bind;

   for (int i = 0; i < num_events; ++i) {
      int namelen = 0;
      int desclen = 0;
      char *name = NULL;
      char *desc = NULL;

      /* Get buffer lengths */
      MPI_T_event_get_info(i, name, &namelen, &verbose, NULL, NULL,
                           &num_elements, &enumtype, &info, desc, &desclen,
                           &bind);

      /* Allocate string buffers */
      name = malloc(sizeof(char) * namelen);
      desc = malloc(sizeof(char) * desclen);
      datatypes = malloc(sizeof(MPI_Datatype) * num_elements);
      displacements = malloc(sizeof(MPI_Aint) * num_elements);

      /* Get event info */
      MPI_T_event_get_info(i, name, &namelen, &verbose, datatypes,
                           displacements, &num_elements, &enumtype, &info, desc,
                           &desclen, &bind);

      infos_keys_t keys = get_info(info);

      /* Print event info */
      printf("Event %i\n"
             "- Name      : %s\n"
             "- Verbosity : %d => %s\n"
             "- Desc      : %s\n"
             "- Bind      : %d => %s\n"
             "- Info keys : %d\n"
             "- Num elems : %d\n",
             i, name, verbose, get_verbose(verbose), desc ? desc : "null", bind,
             get_bind(bind), keys.num, num_elements);

      for (int i = 0; i < keys.num; i++)
         printf("  - Key   : %s\n"
                "  - Value : %s\n"
                "  - Flag  : %d\n",
                keys.key[i], keys.value[i] ? keys.value[i] : "null",
                keys.flag[i]);

      for (int j = 0; j < num_elements; j++)
         printf("  - datatypes[%d]     : %s\n"
                "  - displacements[%d] : %p\n",
                j, get_type(datatypes[j]), j, (void *)displacements[j]);
      printf("\n");

      /* Clean up */
      free(name);
      free(desc);
      free(datatypes);
      free(displacements);
   }
   MPI_Finalize();
   MPI_T_finalize();
}
