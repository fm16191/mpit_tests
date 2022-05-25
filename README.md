# MPI_T Test Library

**MPI_T Test Library** is, as it's name suggests, a grouping of test programs using the `MPI_T` interface.

`MPI_T` is a performance-based tool interface, first introduced in the MPI 3.0 standard.
This interface adds access and control tools on the program environment, as well as, on some MPI events, performance variables and the possibility to implement callbacks.
<!-- These callback functions are implemented by redefining the `MPI_Init` and `MPI_Finalize` calls. -->

This set contains the following : 
- Listing of control variables, with their values and metadatas.
- Listing of performance variables, with their values and metadatas.
- Listing of events, with their metadatas.

## Compilation

- `make`
<!-- - `make -j` for faster compilation time. -->


## Execution

- `./list_cvars`
- `./list_pvars`
- `./list_events`