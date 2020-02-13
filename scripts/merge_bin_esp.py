import os
import argparse

class bin():
    def __init__(self, file_path, addr):
        self.file_path = file_path
        self.file_name = os.path.basename(file_path)
        self.addr = addr
        self.size = self.get_size()
        
    def get_size(self):
        return int(os.path.getsize(self.file_path))

class multiple_bin():
    def __init__(self, name, output_folder):
        self.name = name
        self.output_folder = output_folder
        try:
            os.makedirs(os.path.realpath(self.output_folder))
        except:
            pass
        self.output_path = os.path.realpath(os.path.join(self.output_folder,self.name))
        self.bin_array = []

    def add_bin(self, file_path, addr):
        self.bin_array.append(bin(file_path, addr))

    def sort_bin(self):
        swapped = True
        while swapped:
            swapped = False
            for i in range(len(self.bin_array) - 1):
                if self.bin_array[i].addr > self.bin_array[i + 1].addr:
                    self.bin_array[i], self.bin_array[i + 1] = self.bin_array[i + 1], self.bin_array[i]
                    swapped = True

    def add_bin_to_other_bin(self, previous, binary):
        with open(self.output_path, "a") as output_file:
            output_file.write( '\xff' * (binary.addr-previous))
        print "Add %s from 0x%x to 0x%x (0x%x)"%(binary.file_name, binary.addr, binary.addr+binary.size, binary.size)
        with open(self.output_path, "a") as output_file, open(binary.file_path, "r") as bin_file:
            output_file.write(bin_file.read())
        return binary.addr+binary.size

    def create_bin(self):
        new_start = 0 
        open(self.output_path, "w").close
        for b in self.bin_array:
            new_start = self.add_bin_to_other_bin(new_start, b)

    def check_if_possible(self):
        for i in range(1, len(self.bin_array)):
                if(self.bin_array[i].addr <= (self.bin_array[i-1].addr+self.bin_array[i-1].size)):
                    print self.bin_array[i].addr, (self.bin_array[i-1].addr+self.bin_array[i-1].size)
                    raise Exception("Not possible to create this bin, overlapping between %s and %s"%(self.bin_array[i].file_name, self.bin_array[i-1].file_name))

def main():
    parser = argparse.ArgumentParser(description='Script to merge *.bin file at different position')
    parser.add_argument(
        '--output_name',
        help = 'Output file name', default = "output.bin")
    parser.add_argument(
        '--output_folder', default = "output",
        help = 'Output folder path')
    parser.add_argument(
        '--input_folder', default = "",
        help = 'Input folder path')
    parser.add_argument(
        '--bin_path',nargs='+',required=True,
        help = 'List of bin path, same order as bin_address (space seperated)')
    parser.add_argument(
        '--bin_address',nargs='+',type=lambda x: int(x,0),required=True,
        help = 'List of addr, same order as bin_path (space seperated)')
    parser.add_argument
    args = parser.parse_args()
    mb = multiple_bin(args.output_name, args.output_folder)
    for path,address in zip(args.bin_path, args.bin_address):
        mb.add_bin(os.path.join(args.input_folder, path), address)
    mb.sort_bin()
    mb.check_if_possible()
    mb.create_bin()
    print "app_output.bin generated with success ! (size %u)"%(int(os.path.getsize(mb.output_path)))

if __name__ == "__main__":
    exit(main())
