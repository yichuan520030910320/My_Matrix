#ifndef SJTU_MATRIX_HPP
#define SJTU_MATRIX_HPP
#include <cstddef>
#include <initializer_list>
#include <utility>
#include <iterator>
using std::size_t;
namespace sjtu
{
    template <class T>
    class Matrix
    {
        //声明友元函数
        template<class T1,class U1>
        friend  auto operator*(const Matrix<T1> &mat, const U1 &x);
        template<class U1,class T1>
        friend  auto operator*(const U1 &x, const Matrix<T1> &mat);
        template<class  U1,class V1>
        friend auto operator*(const Matrix<U1> &a, const Matrix<V1> &b);
        template<class  U1,class V1>
        friend auto operator+(const Matrix<U1> &a, const Matrix<V1> &b);
        template<class  U1,class V1>
        friend auto operator-(const Matrix<U1> &a, const Matrix<V1> &b);
    private:
        // your private member variables here.
        std::size_t Row;
        size_t  Column;
        T* matrix=NULL;
    public:
        //Matrix() = default;
        Matrix(){
            Row=0;
            Column=0;
            matrix= nullptr;
        }


        Matrix(size_t n, size_t m, T _init = T())//可加上鲁棒性
        {
            Row=n,Column=m;
            matrix =new T[n*m];
            for (int i = 0; i <n*m ; ++i)
            {
                matrix[i]=_init;
            }
        }
        explicit Matrix(std::pair<size_t, size_t> sz, T _init = T())
        {
            Row=sz.first,Column=sz.second;
            matrix =new T[Row*Column];
            for (int i = 0; i <Row*Column ; ++i) {
                matrix[i]=_init;
            }
        }
        Matrix(const Matrix &o)
        {
            Row=o.Row;
            Column=o.Column;
            matrix=new T[Row*Column];
            for (std::size_t i = 0; i <Row*Column ; ++i)
            {
                matrix[i]=o.matrix[i];
            }
        }
        //拷贝构造
        template <class U>//todo//用于不同类型的构造函数如double 和int
        explicit Matrix(const Matrix<U> &o)
        {
            Row=o.rowLength();
            //std::cout<<Row<<"****"<<std::endl;
            Column=o.columnLength();
            matrix=new T[o.rowLength()*o.columnLength()];
            for (std::size_t i = 0; i <Row ; ++i)
            {
                for (std::size_t j = 0; j <Column ; ++j) {
                  matrix[i*Column+j]=o(i,j);
               }
            }
        }

        Matrix &operator=(const Matrix &o)
        {//std::cout<<"HHHH"<<std::endl;
            //std::cout<<"*****=&"<<std::endl;
            if (this==&o) return *this;
            //clear();
            delete [] matrix;
            Row=o.Row;
            Column=o.Column;
            matrix=new T[Row*Column];
            for (int i = 0; i <Row*Column ; ++i)
            {
                matrix[i]=o.matrix[i];
            }
            return *this;
        }

        template <class U>
        Matrix &operator=(const Matrix<U> &o)
        {//std::cout<<"HHHH"<<std::endl;
          //if (this==&o) return *this;//为什么报错
         // clear();
            delete [] matrix;
            Row=o.rowLength();
            Column=o.columnLength();
            matrix=new T[Row*Column];
            for (int i = 0; i <Row ; ++i)
            {
                for (int j = 0; j <Column ; ++j) {
                    matrix[i*Row+Column]=T(o(i,j));
                }
            }
            return *this;
        }

        Matrix(Matrix &&o) noexcept//:Row(o.Row),Column(o.Column),matrix(o.matrix)
        {//std::cout<<"*****"<<std::endl;
            Row=o.rowLength();
            Column=o.columnLength();
           delete []matrix;
            matrix=o.matrix;
            //delete  [] o.matrix;
            o.matrix= nullptr;
//            return *this;
        }

        Matrix &operator=(Matrix &&o) noexcept//右值引用
        {//std::cout<<"*****"<<std::endl;
            if (this==&o) return *this;
            Row=o.Row;
            Column=o.Column;
            delete [] matrix;
            matrix=o.matrix;
            //
            o.matrix= nullptr;
            return *this;
        }

        ~Matrix() {if (matrix) delete [] matrix;}
        //initializer
        Matrix(std::initializer_list<std::initializer_list<T>> il)
        {
            Row=il.size();
            Column=(il.begin())->size();
            matrix=new T[Row*Column];
            int k=-1;
            for (auto i = il.begin(); i !=il.end() ; ++i) {
                if(Column!=i->size()) {
                    delete []matrix;
                    throw std::invalid_argument("inintializer occur error!");
                }
                for (auto j = i->begin(); j !=i->end() ; ++j) {
                    k++;
                    matrix[k]=(*j);
                }
            }
        }
        //其他的功能
    public:
        size_t rowLength() const {
            return Row;
        }
        size_t columnLength() const {
            return  Column;
        }

        void resize(size_t _n, size_t _m, T _init = T())
        {
            if (_m*_n==Column*Row)
            {
                Row=_n,Column=_m;
            }
            else if (matrix!= nullptr){T* temp=new T[_n*_m];
                for (int i = 0; i <_m*_n ; ++i) {
                    if (i<Column*Row) temp[i]=matrix[i];
                    else temp[i]=_init;
                }
                delete []matrix;
                Row=_n,Column=_m;
                matrix=temp;
                //delete []temp;
            } else if (matrix== nullptr){ Row=_n,Column=_m;
            matrix=new T[Row*Column];
                for (int i=0;i<Row*Column;i++)
                    matrix[i]=_init;
            }
        }

        void resize(std::pair<size_t, size_t> sz, T _init = T())
        {   int _n,_m;
            _n=sz.first,_m=sz.second;
            if (_m*_n==Column*Row)
            {
                Row=_n,Column=_m;
            }
            else if (matrix!= nullptr){T* temp=new T[_n*_m];

                for (int i = 0; i <_m*_n ; ++i) {
                    if (i<Column*Row) temp[i]=matrix[i];
                    else temp[i]=_init;
                }
                delete []matrix;
                Row=_n,Column=_m;
                matrix=temp;
                //temp= nullptr;
            } else{ Row=_n,Column=_m;
                matrix=new T[Row*Column];
                for (int i=0;i<Row*Column;i++)
                    matrix[i]=_init;
            }

//            if (_m*_n==Column*Row)
//            {
//                Row=_n,Column=_m;
//            }
//            else if (_n*_m!=0){T* temp=new T[_n*_m];
//                for (int i = 0; i <_m*_n ; ++i) {
//                    if (i<Column*Row) temp[i]=matrix[i];
//                    else temp[i]=_init;
//                }
//                delete []matrix;
//                Row=_n,Column=_m;
//                matrix=temp;
//                 }
        }


        std::pair<size_t, size_t> size() const
        {
            return std::make_pair(Row,Column);
        };

        void clear()
        {
            delete [] matrix;
            matrix=NULL;
            Row=Column=0;
        }
        //元素获取
    public:
        const T &operator()(size_t i, size_t j) const
        {
            if (i>=Row||j>=Column)  {throw std::invalid_argument("()operator occur error!");}
            else {
                return matrix[(i)*Column+j];
            }
        }

        T &operator()(size_t i, size_t j)
        {
            if (i>=Row||j>=Column) 	 throw std::invalid_argument("()operator occur error!");
            else {
                return matrix[(i)*Column+j];
            }
        }

        Matrix<T> row(size_t i) const
        {if (i<0||i>=Row)  throw std::invalid_argument("()operator row occur error!");
            Matrix <T> temp;
            temp.matrix=new T[Column];
            temp.Column=Column;
            temp.Row=1;
            for (int j = 0; j <Column; ++j)
            {
                temp.matrix[j]=matrix[i*Column+j];
            }
            return temp;
        }

        Matrix<T> column(size_t i) const
        {if (i<0||i>=Column)  throw std::invalid_argument("()operator column occur error!");
            Matrix <T> temp;
            temp.matrix=new T[Row];
            temp.Column=1;
            temp.Row=Row;
            for (int j = 0; j <Row; ++j)
            {
                temp.matrix[j]=matrix[j*Column+i];
            }
            return temp;
        }


    public:

        /*Matrix operator*(const int x)//todo
        {
            for (int i = 0; i <Row ; ++i) {
                for (int j = 0; j <Column ; ++j) {
                   matrix[i*Column+j]*=x;
                }
            }
            return *this;
        }*/


        template <class U>
        bool operator==(const Matrix<U> &o) const
        {if (!(Column==o.columnLength()&&Row==o.rowLength())) return false;
            bool flag=true;
            for (int i = 0; i <Row ; ++i) {
                for (int j = 0; j <Column ; ++j) {
                    if(matrix[i*Column+j]!=o(i,j)) flag= false;
                }
            }
            return flag;
        }

        template <class U>
        bool operator!=(const Matrix<U> &o) const
        {if (!(Column==o.columnLength()&&Row==o.rowLength())) return true;
            bool flag= false;

            for (int i = 0; i <Row ; ++i) {
                for (int j = 0; j <Column ; ++j) {
                    if(matrix[i*Column+j]!=(o(i,j))) flag= true;
                }
            }

            return flag;
        }

        Matrix operator-() const
        {
            Matrix <T> temp;
            temp.matrix=new T[Row*Column];
            temp.Column=Column;
            temp.Row=Row;
            for (int i = 0; i <Row*Column ; ++i) {
                temp.matrix[i]=-matrix[i];
            }
            return temp;

        }

        template <class U>
        Matrix &operator+=(const Matrix<U> &o)
        {
            if (!((Row==o.Row)&&(Column==o.Column))) 	throw std::invalid_argument("operator+= occur error!");
            for (int i = 0; i <Row*Column ; ++i) {
                matrix[i]+=o.matrix[i];
            }
            return *this;

        }

        template <class U>
        Matrix &operator-=(const Matrix<U> &o)
        {
            if (!((Row==o.Row)&&(Column==o.Column))) 	throw std::invalid_argument("operator-= occur error!");
            for (int i = 0; i <Row*Column ; ++i) {
                matrix[i]-=o.matrix[i];
            }
            return *this;
        }
//
        template <class U>
        Matrix &operator*=(const U &x)
        {
            for (int i = 0; i <Row*Column ; ++i) {
                matrix[i]*=x;
            }
            return *this;
        }

        Matrix tran() const
        {
           //Matrix<T> temp(Row,Column);
            Matrix <T> temp;
           // cout << "before" << endl
            temp.matrix=new T[Column*Row];
            temp.Column=Row;
            temp.Row=Column;
            for (int i = 0; i <Row ; ++i) {
                for (int j = 0; j < Column; ++j) {
                    temp.matrix[j*Row+i]=matrix[i*Column+j];
                }
            }
            return temp;
        }

    public: // iterator
        class iterator
        {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = T;
            using pointer           = T *;
            using reference         = T &;
            using size_type         = size_t;
            using difference_type   = std::ptrdiff_t;

            iterator() = default;

            iterator(const iterator &) = default;

            iterator &operator=(const iterator &) = default;

        private:
            size_type size;
            size_type current_type;
            friend class Matrix<T>;
            size_type begin_x,begin_y;
            size_type last_x,last_y;
            size_type matrix_row,matrix_column;
            size_type submatrix_row,submatrix_column;
            size_type now_x,now_y;
            T *storage;


        public:
            difference_type operator-(const iterator &o)
            {
                return (now_x-begin_x)*matrix_column+now_y-begin_y-((o.now_x-o.begin_x)*o.matrix_column+o.now_y-o.begin_y);
            }

            iterator &operator+=(difference_type offset)
            {storage=storage+offset;
                now_x=(now_x*matrix_column+now_y+offset	)/matrix_column;
                now_y=(now_x*matrix_column+now_y+offset	)%matrix_column;
                return *this;
            }

            iterator operator+(difference_type offset) const
            {//std::cout<<"&****&((&&&&("<<std::endl;
                iterator temp;
                temp=*this;
                temp.now_x=(now_x*matrix_column+now_y+offset)/matrix_column;
                temp.now_y=(now_x*matrix_column+now_y+offset)%matrix_column;
                //std::cout<<*temp<<std::endl;
                *temp=*temp+offset;//todo
                //std::cout<<*temp<<"(((("<<std::endl;
                return temp;
            }

            iterator &operator-=(difference_type offset)
            {storage=storage-offset;
                now_x=(now_x*matrix_column+now_y-offset	)/matrix_column;
                now_y=(now_x*matrix_column+now_y-offset	)%matrix_column;
                return *this;
            }

            iterator operator-(difference_type offset) const
            {
                iterator temp;
                temp=*this;
                *this-=offset;
                return temp;
            }//todo have problem here

            iterator &operator++()//learn by libro
            {//std::cout<<"&****&(((555+++++"<<std::endl;
                this->storage-=now_x*matrix_column+now_y;
                int temp=(now_x-begin_x)*submatrix_column+now_y-begin_y+1;
                //std::cout<<temp<<"***"<<submatrix_column<<std::endl;
                now_x=begin_x+temp/submatrix_column;
                now_y=begin_y+temp%submatrix_column;
                this->storage+=now_x*matrix_column+now_y;

//            this->storage+=1;
//            now_x=(now_x*matrix_column+now_y+1	)/matrix_column;
//            now_y=(now_x*matrix_column+now_y+1)%matrix_column;
            return *this;
            }

            iterator operator++(int)
            {//std::cout<<"&****&(((555+++++"<<std::endl;
                iterator temp;
                temp=*this;
                *this=*this+1;
                now_x=(now_x*matrix_column+now_y+1	)/matrix_column;
                now_y=(now_x*matrix_column+now_y+1)%matrix_column;
                return temp;
            }

            iterator &operator--()
            {this->storage-=1;
                now_x=(now_x*matrix_column+now_y-1)/matrix_column;
                now_y=(now_x*matrix_column+now_y-1)%matrix_column;
                return *this;
            }

            iterator operator--(int)
            {
                iterator temp;
                temp=*this;
                *this=*this-1;
                now_x=(now_x*matrix_column+now_y-1)/matrix_column;
                now_y=(now_x*matrix_column+now_y-1)%matrix_column;
                return temp;
            }

            reference operator*() const
            {
                return *storage;
            }

            pointer operator->() const
            {
                return storage;
            }

            bool operator==(const iterator &o) const
            {
                return storage==o.storage;
            }

            bool operator!=(const iterator &o) const
            {
                return  storage!=o.storage;
            }
        };

        iterator begin()
        {
            iterator temp;
            temp.storage=matrix;//头指针
            temp.matrix_column=temp.submatrix_column=Column;
            temp.matrix_row=temp.submatrix_row=Row;
            temp.now_y=temp.now_x=temp.begin_x=temp.begin_y=0;
            temp.size=Row*Column;
            return temp;
        }

        iterator end()
        {
            iterator temp;
            temp.storage=matrix+Row*Column;//头指针
            temp.matrix_column=temp.submatrix_column=Column;
            temp.matrix_row=temp.submatrix_row=Row;
            temp.begin_x=temp.begin_y=0;
            temp.now_x=Row-1;
            temp.now_y=Column-1;
            temp.size=Row*Column;
            return temp;
        }

        std::pair<iterator, iterator> subMatrix(std::pair<size_t, size_t> l, std::pair<size_t, size_t> r)
        {
            iterator a,b;
            a.matrix_row=b.matrix_row=Row;
            a.matrix_column=b.matrix_column=Column;
            a.now_x=l.first,a.now_y=l.second,b.now_x=r.first,b.now_y=r.second;
            a.begin_x=l.first,a.begin_y=l.second,b.last_x=r.first,b.last_y=r.second;
            a.storage=matrix+Column*l.first+l.second;
            b.storage=matrix+Column*r.first+r.second;
            a.submatrix_row=b.submatrix_row=r.first-l.first+1;
            a.submatrix_column=b.matrix_column=r.second-l.second+1;
            return {a,b+1};
        }
    };

}

//
namespace sjtu
{/*template<class T1,class U1>
    friend  auto operator*(const Matrix<T1> &mat, const U1 &x);*/
    template <class T, class U>//存疑这里可以不用decltype吗
    auto operator*(const Matrix<T> &mat, const U &x)//todo
    {   //std::cout<<"****"<<std::endl;
        T t;U u;
        Matrix <decltype(u*t)> temp(mat);
        //temp.matrix=new T[mat.rowLength()*mat.columnLength()];
        //temp.Column=mat.columnLength();
        //temp.Row=mat.rowLength();
        for (std::size_t i = 0; i <mat.rowLength()*mat.columnLength() ; ++i) {
           temp.matrix[i]=temp.matrix[i]*x;
        }
        return temp;
    }

    template <class U, class T>
    auto operator*(const U &x, const Matrix<T> &mat)
    {
        T t;U u;
        Matrix <decltype(u*t)> temp(mat);
        //temp.matrix=new T[mat.rowLength()*mat.columnLength()];
       // temp.Column=mat.columnLength();
        //temp.Row=mat.rowLength();
        for (std::size_t i = 0; i <mat.rowLength()*mat.columnLength(); ++i) {
           temp.matrix[i]=mat.matrix[i]*x;
      }
        return temp;
    }

    template <class U, class V>
    auto operator*(const Matrix<U> &a, const Matrix<V> &b)
    {if (a.Column!=b.Row) throw std::invalid_argument("operator*between matrix occur error!");
        V v;U u;
//        Matrix <decltype(u*v)> temp(a);//todo、、存疑
        typedef decltype(u*v) T;
        Matrix <T> temp;//todo、、存疑
        //std::cout << a.Row * b.Column << std::endl;
        temp.matrix = new T[a.Row * b.Column];
        for(int i = 0; i < a.rowLength()*b.columnLength(); i++)
            temp.matrix[i] = 0;
//        std::cout<<a.columnLength()<<" "<<a.rowLength()<<std::endl;
//        std::cout<<temp.columnLength()<<" "<<temp.rowLength()<<std::endl;
        //delete []a;
//        temp.matrix=new decltype(u*v)[a.Row*b.Column];
        temp.Column=b.Column;
        temp.Row=a.Row;
        for (int i = 0; i <a.Row ; ++i) {
            for (int j = 0; j <b.Column ; ++j) {
//                temp.matrix[i*a.columnLength()+j]=0;
                for (int k = 0; k <a.Column ; ++k) {
                    temp.matrix[i*b.columnLength()+j]+=a.matrix[i*a.columnLength()+k]*b.matrix[k*b.columnLength()+j];
                }
            }
        }
        return temp;
    }

    template <class U, class V>
    auto operator+(const Matrix<U> &a, const Matrix<V> &b)
    {if (!((a.Column==b.Column)&&(a.Row==b.Row))) throw std::invalid_argument("operator+between matrix occur error!");
        V v;U u;
        Matrix <decltype(u+v)> temp(a);
        //temp.matrix=new decltype(u+v)[a.Row*a.Column];
        temp.Column=b.Column;
        temp.Row=a.Row;
        for (int i = 0; i <a.Row ; ++i) {
            for (int j = 0; j <b.Column ; ++j) {
                temp.matrix[i*a.columnLength()+j]=a.matrix[i*a.columnLength()+j]+b.matrix[i*a.columnLength()+j];
            }
        }
        return temp;
    }

    template <class U, class V>
    auto operator-(const Matrix<U> &a, const Matrix<V> &b)
    {
        if (!((a.Column==b.Column)&&(a.Row==b.Row))) throw std::invalid_argument("operator-between matrix occur error!");
        V v;U u;
        Matrix <decltype(u+v)> temp(a);
        //temp.matrix=new decltype(u+v)[a.Row*a.Column];
        temp.Column=b.Column;
        temp.Row=a.Row;
        int i;
       // std::cout<<a.Column<<" "<<a.Row<<std::endl;
        for ( i = 0; i <a.Row ; ++i) {
            for (int j = 0; j <a.Column ; ++j) {
              //  std::cout<<i<<" "<<j<<std::endl;
               // std::cout<<i*a.Column+j<<std::endl;
                temp.matrix[i*a.Column+j]=a.matrix[i*a.Column+j]-b.matrix[i*a.Column+j];
                //std::cout<<a.matrix[i*a.Column+j]<<" "<<b.matrix[i*a.Column+j]<<" "<<temp.matrix[i*a.Column+j]<<std::endl;
            }
            //std::cout<<i<<std::endl;
        }
        //std::cout<<i<<std::endl;
        //std::cout<<i<<std::endl;
        //std::cout<<i<<std::endl;
        return temp;

    }
}

#endif //SJTU_MATRIX_HPP

