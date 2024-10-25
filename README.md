# shitty_bkp
the world's worst backup solution (which fits my needs because im worse) 

# what? 
rsync's a list of folder from a txt file specifically called `backupfile.txt` 

# install deps
on debian / ubuntu <br>

`sudo apt install rsync` (if it dont come with your OS install already)

# usage 
1. write the folder(s) you want to sync on `backupfile.txt` each unique separated by new line
2. run the program and provide the destination `./shittybkp /path/to/destination_folder` (it even works networked computers! just authorize your ssh keypair ! wow)
