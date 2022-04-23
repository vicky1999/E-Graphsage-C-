#include<iostream>
#include<vector>
#include<map>
#include<math.h>

#include "scaler.hpp"

using namespace std;
vector<map<string, any>> StandardScaler::fit(vector<map<string, any>> data, map<string, string> schema, vector<string> columns) {
    cout << "Data size: " << data.size() << " | Columns Size => " << columns.size() << endl;
    vector<double> means = getColumnMeans(data, schema, columns);
    vector<double> std = getSTD(data, schema, columns, means);

    for (int i = 0;i < data.size();i++) {
        for (int j = 0;j < columns.size();j++) {
            if (schema[columns[j]] == "int64") {
                double value = std::any_cast<double>(data[i][columns[j]]);
                data[i][columns[j]] = ((value = means[j]) / std[j]);
            }
            else {
                continue;
            }
        }
    }
    return data;
}

vector<double> StandardScaler::getSTD(vector<map<string, any>> data, map<string, string> schema, vector<string> columns, vector<double> means) {
    vector<double> result;

    for (int i = 0;i < data.size();i++) {
        for (int j = 0;j < columns.size();j++) {
            if (schema[columns[j]] == "int64") {
                double value = std::any_cast<double>(data[i][columns[j]]);
                double x = (value - means[j]);
                if (i == 0) {
                    result.push_back(x * x);
                }
                else {
                    result[j] += (x * x);
                }
            }
            else {
                if (i == 0) {
                    result.push_back(1);
                }
            }
        }
    }

    for (int i = 0;i < columns.size();i++) {
        double std = (result[i] / data.size()); // Need to complete this part!!!
        std = sqrt(std);
        result[i] = std;
    }

    return result;
}

vector<double> StandardScaler::getColumnMeans(vector<map<string, any>> data, map<string, string> schema, vector<string> columns) {
    vector<double> result;

    for (int i = 0;i < data.size();i++) {
        for (int j = 0;j < columns.size();j++) {
            if (schema[columns[j]] == "int64") {
                // cout << "Col: " << columns[j] << " - " << i << endl;
                double value = std::any_cast<double>(data[i][columns[j]]);
                if (i == 0) {
                    result.push_back(value);
                }
                else {
                    result[j] += value;
                }
            }
            else {
                if (i == 0) {
                    result.push_back((double)0);
                }
            }
        }
    }
    for (int i = 0;i < columns.size();i++) {
        result[i] /= (double)data.size();
    }
    cout << "Result size: " << result.size() << endl;
    return result;
}