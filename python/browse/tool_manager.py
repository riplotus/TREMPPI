__author__ = 'adams_000'

import subprocess

class ToolManager:
    _commands = []
    _subprocess = {}

    def add_to_queue(self, command):
        self._commands.append(command)

    # Return progress of currently executed process in percents.
    # If it is done, start a new one from the queue.
    # If all is done, return -1.
    def get_progress(self):
        if self._subprocess is not {} or self._subprocess.poll() is not None:
            if len(self._commands) > 0:
                new_process = self._commands.pop()
                self._subprocess = subprocess.Popen("tremppi " + new_process)
                return 0
            else:
                return -1
        else:
            progress = 0
            for line in self._subprocess.stdout.readlines():
                progress = float(line)
            return progress


    def kill_all(self):
        self._subprocess.kill()
        self._commands = []