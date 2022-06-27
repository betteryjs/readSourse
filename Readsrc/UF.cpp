//
// Created by yjs on 2022/6/27.
//


#include <iostream>
#include <vector>

using namespace std;

class UF {
private:

    vector<int> parents;
    int unionCount = 0; // 合并次数



public:


    UF(int capitalSize) {

        parents.resize(capitalSize);
        // init
        for (int i = 0; i < capitalSize; ++i) {
            parents[i] = i;

        }


    }


    int find(int target) {
        if (parents[target] < 0) return target;
        return parents[target] = find(parents[target]);
    }


    int getUnionCount() const {
        return this->unionCount;
    }

    bool unionCapital(int x, int y) {
        // 将 y 合并到 x 集合中
        int findX = find(x);
        int findY = find(y);
        if (findX != findY) {

            parents[findY] = findX;
            unionCount++;
            return true;
        }
        return false;

    }


};


class WeightedQuickUnionUF {
private:

    vector<int> parents;
    int unionCount = 0; // 合并次数
    vector<int> rank; //  保存树的大小




public:


    WeightedQuickUnionUF(int capitalSize) {

        parents.resize(capitalSize);
        // init
        for (int i = 0; i < capitalSize; ++i) {
            parents[i] = i;
            rank[i] = 1;

        }


    }


    int find(int target) {
        if (parents[target] < 0) return target;
        return parents[target] = find(parents[target]);
    }


    int getUnionCount() const {
        return this->unionCount;
    }

    bool unionCapital(int x, int y) {
        // 将 y 合并到 x 集合中
        int findX = find(x);
        int findY = find(y);
        if (findX != findY) {

            if (rank[findX] <= rank[findY]) {
                parents[findY] = findX;

            } else {
                parents[findX] = findY;

            }
            if (rank[findY] == rank[findX]) rank[findY]++;
            unionCount++;
            return true;
        }
        return false;

    }


};