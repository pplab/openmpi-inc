# OpenMPI-INC

An OpenMPI mod based on 4.0.5.

This mod support In Network Computing (INC) for Huawei CloudEngine 8800 (w/ FPGA).

Developed by Super Computing Center (SCC) in USTC.

## INSTALL

The INC module is precompiled as a static library in INClib/lib and the interface header files are in INClib/include.

The library is compiled by gcc 10.2.0 on an aarch64 platform. It's recommanded to use the same compiler to install openmpi.

At the configure step, add `CFLAGS=-I INClib/include` and `LIBS=-L INClib/lib -linc` to the option, and then compile openmpi as usual.

## USAGE

Set the enviorment variables to run the mpi program:

```bash
export OMPI_MCA_pml=ucx
export OMPI_MCA_btl=^vader,tcp,openib,uct
export UCX_NET_DEVICES=mlx5_0:1
```

## NOTE

The program is developped by Dr. CAI Wenjie and LI Jiaqun, and supervised by Dr. SHEN Yu.
