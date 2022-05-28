#pragma once
//#include "Macros.h"
#include <exception>
#include <memory>
#include <array>
#include <initializer_list>

template<class Type = float, int rows = 3, int cols = 3>
class dMatrix {
public:
	dMatrix() : m_cols(cols), m_rows(rows)
		{
		mp_arr = std::shared_ptr<std::array<std::array<Type, rows>, cols>>(new std::array<std::array<Type, rows>, cols>());
		for (int i = 0; i < m_cols; i++) {
			std::array<Type, rows> col;
			col.fill(Type());
			_get_arr().at(i) = col;
		}
	}
	dMatrix(std::initializer_list<Type> list) : 
		dMatrix<Type, rows, cols>() {
		int msize = static_cast<int>(list.size());
		int count = 0;
		int matr_size = (rows * cols);
		for (auto elem : list) {
			_get_arr().at(count / m_rows).at(count % m_rows) = static_cast<Type>(elem);
			if (++count == matr_size)
				break;
		}
	}
	void set(Type val, int row, int col = 0) {
		_get_arr().at(col).at(row) = val;
	}
	Type& at(int arow, int acol = 0)  const {
		return static_cast<Type&>(_get_arr().at(acol).at(arow));
	}
	template <int src_rows, int src_cols>
	inline dMatrix<Type, src_rows, src_cols>& operator=(dMatrix<Type, src_rows, src_cols>&& rsrc) {
		if (this != &rsrc) {
		mp_arr = rsrc.get_arr_ptr();
		m_cols = rsrc.get_cols();
		m_rows = rsrc.get_rows();
		}
		return *this;
	}
	template <int src_rows, int src_cols>
	inline dMatrix(const dMatrix<Type, src_rows, src_cols>& src)
		: m_cols(cols), m_rows(rows)
	{
		mp_arr = std::shared_ptr<std::array<std::array<Type, rows>, cols>>(new std::array<std::array<Type, rows>, cols>());
		for (int i = 0; i < m_cols; i++) {
			std::array<Type, rows> col;
			col.fill(Type());
			_get_arr().at(i) = col;
		}
		for (int col = 0; col < src_cols; col++) {
			for (int row = 0; row < src_rows; row++) {
				this->at(row, col) = src.at(row, col);
			}
		}
	}
	inline dMatrix<Type, rows, cols>& copy(dMatrix<Type, rows, cols>& src) {
		for (int col = 0; col < src.get_cols(); col++) {
			for (int row = 0; row < src.get_rows(); row++) {
				this->at(row, col) = src.at(row, col);
			}
		}
		return *this;
	}

	inline dMatrix<Type, rows, cols> getCopy() {
		dMatrix<Type, rows, cols> result;
		for (int col = 0; col < m_cols; col++) {
			for (int row = 0; row < m_rows; row++) {
				result.at(row, col) = this->at(row, col);
			}
		}
		return result;
	}

	inline dMatrix<Type, rows, cols>& transpose() {
		if (rows != cols)
			throw std::runtime_error(std::string("Transposition of this matrix is prohibited! (Nonequal sides)"));
		dMatrix<Type, rows, cols> result;
		for (int col = 0; col < m_cols; col++) {
			for (int row = 0; row < m_rows; row++) {
				result.at(row, col) = this->at(col, row);
			}
		}
		this->copy(result);
		return *this;
	}

	inline dMatrix<Type, rows, cols>& inverse() {
		return this->copy(operator-(*this));
	}

	// Do not try to get minor of a 1x1 matrix pls
	inline dMatrix<Type, rows-1, cols-1> getMinor(int base_row, int base_col) {
		dMatrix<Type, rows - 1, cols - 1> result;
		int next_row = 0;
		int next_col = 0;
		for (int col = 0; col < m_cols; col++) {
			for (int row = 0; row < m_rows; row++) {
				if (row != base_row and col != base_col) {
					result.at(next_row++, next_col) = this->at(row, col);
					if (next_row >= rows - 1) {
						next_row = 0; ++next_col;
					}
				}
			}
		}
		return result;
	}

	// Used to unwrap the value in the 1x1 matrix
	inline Type _unwrap() {
		return _get_arr()[0][0];
	}

	template <int src_rows, int src_cols>
	inline dMatrix<Type, src_rows, cols> operator*(dMatrix<Type, src_rows, src_cols>& src) {
		if (m_rows != src.get_cols())
			throw std::runtime_error(std::string("Cant multiply matrices of these sizes! (Or you did try Matrix * Vector? -> Should be Vector * Matrix)"));
		dMatrix<Type, src_rows, cols> result;  // Yesli viletel, to ti lox yopta
		for (int rescol = 0; rescol < cols; rescol++) {
			for (int resrow = 0; resrow < src_rows; resrow++) {
				for (int src_row = 0 ; src_row < src.get_rows(); src_row++) {
					auto base = this->at(src_row, rescol);
					auto other = src.at(resrow, src_row);
					result.at(resrow, rescol) += (base * other);
				}	
			}
		}
		return result;
	};
	template <int src_rows, int src_cols>
	inline dMatrix<Type, src_rows, cols> operator*(dMatrix<Type, src_rows, src_cols>&& src) {
		if (m_rows != src.get_cols())
			throw std::runtime_error(std::string("Cant multiply matrices of these sizes! (Or you did try Matrix * Vector? Should be Vector * Matrix)"));
		dMatrix<Type, src_rows, cols> result;  // Yesli viletel, to ti lox yopta
		for (int rescol = 0; rescol < cols; rescol++) {
			for (int resrow = 0; resrow < src_rows; resrow++) {
				for (int src_row = 0; src_row < src.get_rows(); src_row++) {
					auto base = this->at(src_row, rescol);
					auto other = src.at(resrow, src_row);
					result.at(resrow, rescol) += (base * other);
				}
			}
		}
		return result;
	};
	template <int src_rows, int src_cols>
	inline dMatrix<Type, rows, cols>& operator*=(dMatrix<Type, src_rows, src_cols>& src) {
		if (m_rows != src.get_cols())
			throw std::runtime_error(std::string("Cant multiply matrices of these sizes! (Or you did try Matrix * Vector? Should be Vector * Matrix)"));
		dMatrix<Type, rows, cols> result;  // Yesli viletel, to ti lox yopta
		for (int rescol = 0; rescol < cols; rescol++) {
			for (int resrow = 0; resrow < rows; resrow++) {
				for (int src_row = 0; src_row < src.get_rows(); src_row++) {
					auto base = this->at(src_row, rescol);
					auto other = src.at(resrow, src_row);
					result.at(resrow, rescol) += (base * other);
				}
			}
		}
		this->copy(result);
		return *this;
	};
	template <int src_rows, int src_cols>
	inline dMatrix<Type, rows, cols>& operator*=(dMatrix<Type, src_rows, src_cols>&& src) {
		if (m_rows != src.get_cols())
			throw std::runtime_error(std::string("Cant multiply matrices of these sizes! (Or you did try Matrix * Vector? Should be Vector * Matrix)"));
		dMatrix<Type, rows, cols> result;  // Yesli viletel, to ti lox yopta
		for (int rescol = 0; rescol < cols; rescol++) {
			for (int resrow = 0; resrow < rows; resrow++) {
				for (int src_row = 0; src_row < src.get_rows(); src_row++) {
					auto base = this->at(src_row, rescol);
					auto other = src.at(resrow, src_row);
					result.at(resrow, rescol) += (base * other);
				}
			}
		}
		this->copy(result);
		return *this;
	};
	~dMatrix() {
	}
	template <int res_rows = rows, int res_cols = cols>
	inline dMatrix<Type,res_rows, res_cols> operator+(dMatrix<Type, res_rows, res_cols>& src) const {
		if (m_rows != src.get_rows() or m_cols != src.get_cols())
			throw std::runtime_error(std::string("Incompatible matrices!"));
		dMatrix<Type, res_rows, res_cols> result;
		for (int bcol = 0; bcol < res_cols; bcol++) {
			for (int brow = 0; brow < res_rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) + src.at(brow, bcol);
			}
		}
	return result;
	};
	template <int res_rows = rows, int res_cols = cols>
	inline dMatrix<Type, res_rows, res_cols> operator+(dMatrix<Type, res_rows, res_cols>&& src) const {
		if (m_rows != src.get_rows() or m_cols != src.get_cols())
			throw std::runtime_error(std::string("Incompatible matrices!"));
		dMatrix<Type, res_rows, res_cols> result;
		for (int bcol = 0; bcol < res_cols; bcol++) {
			for (int brow = 0; brow < res_rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) + src.at(brow, bcol);
			}
		}
		return result;
	};
	template <int res_rows = rows, int res_cols = cols>
	inline dMatrix<Type, res_rows, res_cols>& operator+=(dMatrix<Type, res_rows, res_cols>& src) const {
		if (m_rows != src.get_rows() or m_cols != src.get_cols())
			throw std::runtime_error(std::string("Incompatible matrices!"));
		dMatrix<Type, res_rows, res_cols> result;
		for (int bcol = 0; bcol < res_cols; bcol++) {
			for (int brow = 0; brow < res_rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) + src.at(brow, bcol);
			}
		}
		this->copy(result);
		return *this;
	};
	template <int res_rows = rows, int res_cols = cols>
	inline dMatrix<Type, res_rows, res_cols>& operator+=(dMatrix<Type, res_rows, res_cols>&& src) const {
		if (m_rows != src.get_rows() or m_cols != src.get_cols())
			throw std::runtime_error(std::string("Incompatible matrices!"));
		dMatrix<Type, res_rows, res_cols> result;
		for (int bcol = 0; bcol < res_cols; bcol++) {
			for (int brow = 0; brow < res_rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) + src.at(brow, bcol);
			}
		}
		this->copy(result);
		return *this;
	};
	template <int res_rows = rows, int res_cols = cols>
	inline dMatrix<Type, res_rows, res_cols> operator-(dMatrix<Type, res_rows, res_cols>& src) const {
		if (m_rows != src.get_rows() or m_cols != src.get_cols())
			throw std::runtime_error(std::string("Incompatible matrices!"));
		dMatrix<Type, res_rows, res_cols> result;
		for (int bcol = 0; bcol < res_cols; bcol++) {
			for (int brow = 0; brow < res_rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) - src.at(brow, bcol);
			}
		}
		return result;
	};
	template <int res_rows = rows, int res_cols = cols>
	inline dMatrix<Type, res_rows, res_cols>& operator-=(dMatrix<Type, res_rows, res_cols>& src) {
		if (m_rows != src.get_rows() or m_cols != src.get_cols())
			throw std::runtime_error(std::string("Incompatible matrices!"));
		dMatrix<Type, res_rows, res_cols> result;
		for (int bcol = 0; bcol < res_cols; bcol++) {
			for (int brow = 0; brow < res_rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) - src.at(brow, bcol);
			}
		}
		this->copy(result);
		result* this;
	};
	template <int res_rows = rows, int res_cols = cols>
	inline dMatrix<Type, res_rows, res_cols>& operator-=(dMatrix<Type, res_rows, res_cols>&& src) {
		if (m_rows != src.get_rows() or m_cols != src.get_cols())
			throw std::runtime_error(std::string("Incompatible matrices!"));
		dMatrix<Type, res_rows, res_cols> result;
		for (int bcol = 0; bcol < res_cols; bcol++) {
			for (int brow = 0; brow < res_rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) - src.at(brow, bcol);
			}
		}
		this->copy(result);
		result* this;
	};
	inline dMatrix<Type, rows, cols> operator+(Type src) const {
		dMatrix<Type, rows, cols> result;
		for (int bcol = 0; bcol < cols; bcol++) {
			for (int brow = 0; brow < rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) + src;
			}
		}
		return result;
	};
	inline dMatrix<Type, rows, cols>& operator+=(Type src) {
		dMatrix<Type, rows, cols> result;
		for (int bcol = 0; bcol < cols; bcol++) {
			for (int brow = 0; brow < rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) + src;
			}
		}
		this->copy(result);
		return *this;
	};
	inline dMatrix<Type, rows, cols> operator-(Type src) const {
		dMatrix<Type, rows, cols> result;
		for (int bcol = 0; bcol < cols; bcol++) {
			for (int brow = 0; brow < rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) - src;
			}
		}
		return result;
	};
	inline dMatrix<Type, rows, cols> operator-=(Type src) {
		dMatrix<Type, rows, cols> result;
		for (int bcol = 0; bcol < cols; bcol++) {
			for (int brow = 0; brow < rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) - src;
			}
		}
		this->copy(result);
		return *this;
	};
	inline dMatrix<Type, rows, cols> operator*(Type src) const {
		dMatrix<Type, rows, cols> result;
		for (int bcol = 0; bcol < cols; bcol++) {
			for (int brow = 0; brow < rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) * src;
			}
		}
		return result;
	};
	inline dMatrix<Type, rows, cols>& operator*=(Type src){
		dMatrix<Type, rows, cols> result;
		for (int bcol = 0; bcol < cols; bcol++) {
			for (int brow = 0; brow < rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) * src;
			}
		}
		this->copy(result);
		return *this;
	};
	inline dMatrix<Type, rows, cols> operator/(Type src) const {
		dMatrix<Type, rows, cols> result;
		for (int bcol = 0; bcol < cols; bcol++) {
			for (int brow = 0; brow < rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) / src;
			}
		}
		return result;
	};
	inline dMatrix<Type, rows, cols>& operator/=(Type src){
		dMatrix<Type, rows, cols> result;
		for (int bcol = 0; bcol < cols; bcol++) {
			for (int brow = 0; brow < rows; brow++) {
				result.at(brow, bcol) = _get_arr().at(bcol).at(brow) / src;
			}
		}
		this->copy(result);
		return *this;
	};
	inline int get_rows() const {
		return m_rows;
	}
	inline int get_cols() const {
		return m_cols;
	}
	inline std::shared_ptr<std::array<std::array<Type, rows>, cols>> get_arr_ptr() const{
		return mp_arr;
	}
	inline std::shared_ptr<std::array<std::array<Type, rows>, cols>> get_arr_ptr(){
		return mp_arr;
	}
	inline std::array<std::array<Type, rows>, cols>& _get_arr() const{
		return *mp_arr;
	}
	template <int src_rows, int src_cols>
	inline std::array<std::array<Type, src_rows>, src_cols>& _get_arr() {
		return *mp_arr;
	}
protected:
	std::shared_ptr<std::array<std::array<Type, rows>, cols>> mp_arr;
	int m_rows;
	int m_cols;
};

template <class Type>
inline Type det2(dMatrix<Type, 2, 2> src) {
	return src._get_arr()[0][0] * src._get_arr()[1][1] - src._get_arr()[1][0] * src._get_arr()[0][1];
}

template <class Type>
inline Type det3(dMatrix<Type, 3, 3> src) {
	Type result{};
	for (int row = 0; row < 3; row++) {
		result += det2<Type>(src.getMinor(row, 0)) * src.at(row, 0) * ((row % 2) ? -1 : 1);
	}
	return result;
}
 

template <class Type>
inline dMatrix<Type, 3, 3> findCofactors(dMatrix<Type, 3, 3>& src) {
	dMatrix<Type, 3, 3> l_cofactors;
	for (int col = 0; col < src.get_cols(); col++) {
		for (int row = 0; row < src.get_rows(); row++) {
			l_cofactors.at(row, col) = det2<Type>(src.getMinor(row, col)) * (((row + col * src.get_rows()) % 2) ? -1 : 1);
		}
	}
	return l_cofactors;
}

// Matrix inversion
template <class Type>
inline dMatrix<Type, 3, 3> operator-(dMatrix<Type, 3, 3>& src) {
	auto l_cofactors = findCofactors(src);
	auto det = det3(src);
	if (not det)
		throw std::runtime_error(std::string("Inverse matrix does not exist!"));
	return l_cofactors.transpose() * (1/ det);
}
