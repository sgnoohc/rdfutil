#!/bin/bash

FILELIST="files.txt"
OUTDIR="/blue/phy4905/share/p.chang/ntuples"
EXECUTABLE="./create_fourlepton_ntuple"

# Count number of files
NFILES=$(grep -cv '^\s*#\|^\s*$' "$FILELIST")

if [ "$NFILES" -eq 0 ]; then
    echo "No files found in $FILELIST"
    exit 1
fi

echo "Submitting $NFILES jobs from $FILELIST"

mkdir -p "$OUTDIR"
mkdir -p logs

sbatch <<EOF
#!/bin/bash
#SBATCH --job-name=4lep
#SBATCH --account=avery
#SBATCH --qos=avery
#SBATCH --array=1-${NFILES}
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem=8gb
#SBATCH --time=04:00:00
#SBATCH --output=logs/4lep_%A_%a.out
#SBATCH --error=logs/4lep_%A_%a.err

# Get the file for this array task (skip comments and blank lines)
INPUT_FILE=\$(grep -v '^\s*#\|^\s*$' "$FILELIST" | sed -n "\${SLURM_ARRAY_TASK_ID}p")

if [ -z "\$INPUT_FILE" ]; then
    echo "ERROR: no file at index \${SLURM_ARRAY_TASK_ID}"
    exit 1
fi

INDEX=\$(printf "%04d" \$SLURM_ARRAY_TASK_ID)
OUTPUT_FILE="${OUTDIR}/data_Run2_4l_\${INDEX}.root"

echo "Task \${SLURM_ARRAY_TASK_ID}: \$INPUT_FILE -> \$OUTPUT_FILE"

# Write a temporary single-file list for this task
TMPLIST=\$(mktemp /tmp/filelist_\${SLURM_ARRAY_TASK_ID}_XXXXXX.txt)
echo "\$INPUT_FILE" > "\$TMPLIST"

${EXECUTABLE} "\$TMPLIST" "\$OUTPUT_FILE"
RETVAL=\$?

rm -f "\$TMPLIST"
exit \$RETVAL
EOF

echo "Submitted array job with $NFILES tasks"
