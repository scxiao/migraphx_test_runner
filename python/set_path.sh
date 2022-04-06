#export LD_LIBRARY_PATH=/home/scxiao/Workplace/projects/AMDMIGraphX/deps_py/lib:/home/scxiao/Workplace/software/migraphlibs:$LD_LIBRARY_PATH

#if [ $# -ne 1 ]; then
#    echo "Usage: set_path.sh repo_name"
#    exit 0
#fi

repo=AMDMIGraphX
echo $repo

export LD_LIBRARY_PATH=/home/scxiao/Workplace/projects/$repo/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/scxiao/Workplace/projects/$repo/build/lib:$PYTHONPATH

