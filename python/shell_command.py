import subprocess

def shell_command(command):
   try:
      subprocess.run(command, shell=True, check=True)
   except subprocess.CalledProcessError as e:
      print(f"Error: {e}")
