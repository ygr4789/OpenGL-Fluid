#pragma once
#include <vector>
#include "particle.h"

class Cell {
public:
	Cell() {
		pa = new Particle[10];
		capacity = 10;
		size = 0;
	}
	~Cell() {
		delete[] pa;
	}
	void insert(Particle p) {
		if (capacity == size) {
			Particle* temp = new Particle[capacity * 2];
			for (int i = 0; i < capacity; i++) {
				temp[i] = pa[i];
			}
			capacity *= 2;
			delete[] pa;
			pa = temp;
		}
		pa[size++] = p;
		s = size;
	}
	Particle pop() {
		return pa[--s];
	}
	int getSize() {
		return s;
	}
	void resetS() {
		s = size;
	}
private:
	Particle* pa;
	int capacity;
	int size;
	int s;
};

class Hash {
public:
	Hash() {
		nx = BOUNDARY * 2;
		ny = BOUNDARY * 2;
		nz = BOUNDARY * 2;
		n = (nx + 1) * (ny + 1) * (nz + 1);
		cellCount = new int[n];
		for (int i = 0; i < n; i++) {
			cellCount[i] = 0;
		}
		cells = new Cell[n];
	}
	~Hash() {
		delete[] cellCount;
		delete[] cells;
	}
	void hashing(vector<Particle> pv) {
		int i;
		for (auto& p : pv) {
			int xi = floor(p.pos.x + BOUNDARY + EPSILON);
			int yi = floor(p.pos.y + BOUNDARY + EPSILON);
			int zi = floor(p.pos.z + BOUNDARY + EPSILON);
			i = (xi * ny + yi) * nz + zi;
			cellCount[i]++;
			cells[i].insert(p);
		}
	}
	vector<Particle> neighbor(Particle p) {
		int xi = floor(p.pos.x + BOUNDARY + EPSILON);
		int yi = floor(p.pos.y + BOUNDARY + EPSILON);
		int zi = floor(p.pos.z + BOUNDARY + EPSILON);
		int i = (xi * ny + yi) * nz + zi;
		int n = 0;
		vector<Particle> pv;
		while (cells[i].getSize() > 0) {
			pv.push_back(cells[i].pop());
			n++;
		}
		cells[i].resetS();
		if (i > 0 && cellCount[i - 1] > 0) {
			while (cells[i-1].getSize() > 0) {
				pv.push_back(cells[i-1].pop());
				n++;
			}
			cells[i-1].resetS();
		}
		if ((i+1) < n && cellCount[i + 1] > 0) {
			while (cells[i+1].getSize() > 0) {
				pv.push_back(cells[i+1].pop());
				n++;
			}
			cells[i + 1].resetS();
		}
		if ((i+nz) < n && cellCount[i+nz] > 0) {
			while (cells[i+nz].getSize() > 0) {
				pv.push_back(cells[i+nz].pop());
				n++;
			}
			cells[i + nz].resetS();
		}
		if ((i - nz) >= 0 && cellCount[i - nz]) {
			while (cells[i-nz].getSize() > 0) {
				pv.push_back(cells[i-nz].pop());
				n++;
			}
			cells[i - nz].resetS();
		}
		if ((i + nz * ny) < n && cellCount[i + nz * ny]) {
			while (cells[i+nz*ny].getSize() > 0) {
				pv.push_back(cells[i+nz*ny].pop());
				n++;
			}
			cells[i + nz * ny].resetS();
		}
		if ((i - nz * ny) >= 0 && cellCount[i - nz * ny]) {
			while (cells[i-nz*ny].getSize() > 0) {
				pv.push_back(cells[i-nz*ny].pop());
				n++;
			}
			cells[i - nz * ny].resetS();
		}
		return pv;
	}
	void print() {
		for (int i = 0; i < n; i++) {
			printf("%d : %d ", i, cellCount[i]);
			if (i % 10 == 0) printf("\n");
		}
	}
private:
	//Particle* particles;
	Cell* cells;
	int* cellCount;
	int nx;
	int ny;
	int nz;
	int n;
};