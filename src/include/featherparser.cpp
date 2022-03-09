#include <iostream>
#include <vector>

#include "featherparser.hpp"

#include <arrow/api.h>
#include <arrow/dataset/file_base.h>
#include <arrow/ipc/api.h>

using namespace std;

void FeatherParser::FeatherParser()
{
    cout << "Feather Parser Constructor!!!" << endl;
}

std::shared_ptr<arrow::Table> FeatherParser::readFile(string uri)
{
    string root_path;
    arrow::Status st;
    arrow::MemoryPool *pool = arrow::default_memory_pool();
    auto file_system = arrow::fs::FileSystemFromUri(uri, &root_path).ValueOrDie();
    cout << root_path << endl;
    std::shared_ptr<arrow::io::RandomAccessFile> input = file_system->OpenInputFile(root_path).ValueOrDie();
    std::shared_ptr<arrow::ipc::feather::Reader> feather_reader = arrow::ipc::feather::Reader::Open(input).ValueOrDie();
    std::shared_ptr<arrow::Table> table;
    st = feather_reader->Read(&table);
    if (!st.ok())
    {
        throw "Something went wrong while Reading Feather File!!!";
    }
    return table;
}
