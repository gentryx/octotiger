/*
 * util.cpp
 *
 *  Created on: Apr 20, 2016
 *      Author: dmarce1
 */

//#include <sse_mathfun.h>
#include <cmath>
#include <stdio.h>
#include <functional>
#include <unordered_map>
#include <memory>
#include <stack>
#include <atomic>
#include <mpi.h>
#include "defs.hpp"

using real = double;



int file_copy(const char* fin, const char* fout) {
	constexpr size_t chunk_size = 1024;
	char buffer[chunk_size];
	FILE* fp_in = fopen(fin, "rb");
	FILE* fp_out = fopen(fout, "wb");
	if (fp_in == NULL) {
		return 1;
	}
	if (fp_out == NULL) {
		return 2;
	}
	size_t bytes_read;
	while ((bytes_read = fread(buffer, sizeof(char), chunk_size, fp_in)) != 0) {
		fwrite(buffer, sizeof(char), bytes_read, fp_out);
	}
	fclose(fp_in);
	fclose(fp_out);
}

bool find_root(std::function<double(double)>& func, double xmin, double xmax,
		double& root, double toler) {
	double xmid;
	const auto error = [](const double _xmax, const double _xmin) {
		return (_xmax - _xmin) / (std::abs(_xmax) + std::abs(_xmin))*2.0;
	};
	double xmin0 = xmin;
	double xmax0 = xmax;
	while (error(xmax,xmin) > toler) {
		xmid = (xmax + xmin) / 2.0;
		if (func(xmid) * func(xmax) < 0.0) {
			xmin = xmid;
		} else {
			xmax = xmid;
		}
	}
	root = xmid;
	if( error(root,xmin0) < 10.0*toler || error(xmax0,root) < 10.0*toler ) {
		return false;
	} else {
		return true;
	}
}

