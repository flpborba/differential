#!/bin/bash
#SBATCH --job-name ddt
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=80
#SBATCH --time=24:00:00
#SBATCH --output=dickson_lc_3-32-32.txt

cd $SLURM_SUBMIT_DIR

start_deg=3
end_deg=32
repeat=32

rm -rf ./build

source load-modules.sh
source build.sh Release icc icpc
source run.sh $start_deg $end_deg $repeat
