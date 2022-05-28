#define INTERNAL_DMAKE_MATR_OP_TYPE(op)  \
	inline dMatrix<Type, rows, cols> operator op(Type src) const { \
		dMatrix<Type, rows, cols> result; \
		for (int bcol = 0; bcol < cols; bcol++) { \
			for (int brow = 0; brow < rows; brow++) { \
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) op src; \
			} \
		} \
		return result; \
	}; \
	inline dMatrix<Type, rows, cols>& operator op##=(Type src) { \
		dMatrix<Type, rows, cols> result; \
		for (int bcol = 0; bcol < cols; bcol++) { \
			for (int brow = 0; brow < rows; brow++) { \
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) op src; \
			} \
		} \
		this->copy(result); \
		return *this; \
	};
#define INTERNAL_DMAKE_MATR_OP_MATR_BASE(op, isopeq, isrsrc, isconst) \
	template <int res_rows = rows, int res_cols = cols> \
	inline dMatrix<Type, res_rows, res_cols> operator op##isopeq(dMatrix<Type, res_rows, res_cols> &##isrsrc src) isconst { \
		if (m_rows != src.get_rows() or m_cols != src.get_cols()) \
			throw std::runtime_error(std::string("Incompatible matrices!")); \
		dMatrix<Type, res_rows, res_cols> result; \
		for (int bcol = 0; bcol < res_cols; bcol++) { \
			for (int brow = 0; brow < res_rows; brow++) { \
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow)  op src.at(brow, bcol); \
			} \
		} \
		return result; \
	}; 

#define INTERNAL_DMAKE_MATR_OP_MATR(op) INTERNAL_DMAKE_MATR_OP_MATR_BASE(op, , , const) INTERNAL_DMAKE_MATR_OP_MATR_BASE(op, =, , ) INTERNAL_DMAKE_MATR_OP_MATR_BASE(op, =, &, )