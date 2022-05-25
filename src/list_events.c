/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

/* Test MPI_T_xxx_get_index() for cvars, pvars and categories.
 * 2021-02-17: added events
 */
#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>

static int verbose = 0;

static void print_info(MPI_Info info)
{
   if (info == MPI_INFO_NULL) {
      printf("info :\t\tNULL\n");
      return;
   }

   int nkeys;

   MPI_Info_get_nkeys(info, &nkeys);
   printf("nkeys : %d\n", nkeys);

   for (int i = 0; i < nkeys; i++) {
      char *key = NULL, *value = NULL;
      int valuelen = 0, flag;
      MPI_Info_get(info, key, valuelen, value, &flag);
      printf("key : %s"
             "value : [%s](%d)"
             "flag : %d\n",
             key ? key : "", value ? value : "", valuelen, flag);
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
      int namelen = 256;
      int desclen = 256;
      char *name = NULL;
      char *desc = NULL;

      /* Get buffer lengths */
      MPI_T_event_get_info(i, name, &namelen, &verbose, NULL, NULL,
                           &num_elements, NULL, NULL, desc, &desclen, NULL);

      /* Allocate string buffers */
      name = malloc(sizeof(char) * namelen);
      desc = malloc(sizeof(char) * desclen);
      datatypes = malloc(sizeof(MPI_Datatype) * num_elements);
      displacements = malloc(sizeof(MPI_Aint) * num_elements);

      /* Get event info */
      MPI_T_event_get_info(i, name, &namelen, &verbose, datatypes,
                           displacements, &num_elements, &enumtype, &info, desc,
                           &desclen, &bind);

      /* Print event info */
      if (rank == 0) {
         printf("* %d\t\t[%s](%d)\n"
                "Verbose\t\t%d\n"
                "desc(%d) \t%s\n"
                "Bind\t\t%d\n",
                i, name, namelen, verbose, desclen, desc, bind);

         print_info(info);
         printf("\n");

         printf("datatypes/displacements num_elements : %d\n", num_elements);
         for (int j = 0; j < num_elements; j++)
            printf("datatypes[%d] :     %s\n"
                   "displacements[%d] : %p\n",
                   j, get_type(datatypes[j]), j, (void *)displacements[j]);
         printf("\n");
      }
   }
   MPI_Finalize();
   MPI_T_finalize();
}
