class Recorder:
    __records = {};

    def add(self, ptr, file, line):
        self.__records[ptr] = [file, line];

    def remove(self, ptr):
        try:
            self.__records.pop(ptr);
        except Exception :
            pass;

    def dump(self):
        str = "";
        for ptr, info in self.__records.items():
            str += "address: 0x%x, file: %s, line:%d\n" % (ptr, info[0], info[1]);
        return str;

    def dumpToFile(self, fileName):
        with open(fileName, "w") as f:
            f.write(self.dump());