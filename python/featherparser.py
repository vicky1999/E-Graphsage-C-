import argparse
import os

from pyarrow import feather

def make_csv(filename):
    basename = filename.split('.')[0];
    data = feather.read_feather('../'+filename)
    data.to_csv(f'../data/{basename}.csv', index=False)
    print(f'csv file for {filename} created!')

def main():
    node_file = ''
    ip_file = ''

    msg = "E-GraphSAGE"
    parser = argparse.ArgumentParser(description = msg)
    parser.add_argument("-n", "--Node", help = "Enter Node input file")
    parser.add_argument("-i", "--IP", help = "Enter IP input file")

    args = parser.parse_args()

    if(args.Node):
        node_file = args.Node;
    if(args.IP):
        ip_file = args.IP;

    make_csv(node_file)
    make_csv(ip_file)


    
if(__name__ == '__main__'):
    main()