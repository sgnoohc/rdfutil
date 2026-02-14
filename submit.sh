#!/bin/bash

# ---- Configurable parameters ----
FILELIST="files.txt"
OUTDIR="/blue/phy4905/share/p.chang/ntuples"
EXECUTABLE="./create_fourlepton_ntuple"
FILES_PER_JOB=1
CPUS_PER_TASK=4
MEM="8gb"
TIME="04:00:00"
# ----------------------------------

# Build clean file list in the working directory (not /tmp, which differs per node)
CLEAN_FILELIST="$(pwd)/.filelist_clean.txt"
grep -v '^\s*#\|^\s*$' "$FILELIST" > "$CLEAN_FILELIST"
TOTAL_FILES=$(wc -l < "$CLEAN_FILELIST")

if [ "$TOTAL_FILES" -eq 0 ]; then
    echo "No files found in $FILELIST"
    exit 1
fi

NJOBS=$(( (TOTAL_FILES + FILES_PER_JOB - 1) / FILES_PER_JOB ))

echo "Total files: $TOTAL_FILES"
echo "Files per job: $FILES_PER_JOB"
echo "CPUs per task: $CPUS_PER_TASK"
echo "Number of jobs: $NJOBS"

mkdir -p "$OUTDIR"
mkdir -p logs

sbatch <<EOF
#!/bin/bash
#SBATCH --job-name=4lep
#SBATCH --account=avery
#SBATCH --qos=avery
#SBATCH --array=1-${NJOBS}
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=${CPUS_PER_TASK}
#SBATCH --mem=${MEM}
#SBATCH --time=${TIME}
#SBATCH --output=logs/4lep_%A_%a.out
#SBATCH --error=logs/4lep_%A_%a.err

source setuproot.sh

# Slice the file list for this job
START_LINE=\$(( (SLURM_ARRAY_TASK_ID - 1) * ${FILES_PER_JOB} + 1 ))
END_LINE=\$(( START_LINE + ${FILES_PER_JOB} - 1 ))

TMPLIST=\$(mktemp /tmp/filelist_\${SLURM_ARRAY_TASK_ID}_XXXXXX.txt)
sed -n "\${START_LINE},\${END_LINE}p" ${CLEAN_FILELIST} > "\$TMPLIST"

NLINES=\$(wc -l < "\$TMPLIST")
if [ "\$NLINES" -eq 0 ]; then
    echo "ERROR: no files for task \${SLURM_ARRAY_TASK_ID}"
    rm -f "\$TMPLIST"
    exit 1
fi

INDEX=\$(printf "%04d" \$SLURM_ARRAY_TASK_ID)
OUTPUT_FILE="${OUTDIR}/data_Run2_4l_\${INDEX}.root"

echo "Task \${SLURM_ARRAY_TASK_ID}: \$NLINES files -> \$OUTPUT_FILE (threads=${CPUS_PER_TASK})"
cat "\$TMPLIST"

${EXECUTABLE} "\$TMPLIST" "\$OUTPUT_FILE" ${CPUS_PER_TASK}
RETVAL=\$?

rm -f "\$TMPLIST"
exit \$RETVAL
EOF

echo "Submitted array job with $NJOBS tasks"
