#include<iostream >
#include<string>
#include <typeinfo>
using namespace std;

#pragma GCC diagnostic ignored "-Wreturn-type"

//Structure
struct Coefficient{
    double intercept;
    double slope;
};

//template
template <typename dt>
//Class
class matrix{

private:
    dt **data;
    int nrow;
    int ncol;    

public:

    //Constructor
    //Function overloading   
    matrix(int n =0){
        nrow=n;
        ncol=n;
        allocateMemory();
    }
    matrix(int r, int c){
        nrow = r;
        ncol = c;
        if(nrow != 0 && ncol != 0)
            allocateMemory();
    }
    matrix(dt *a, int len, int row, int col, bool byrow = true){
        nrow = row;
        ncol = col;
        allocateMemory();
        setData(a, len, byrow);
    }
    //Copy constructor (deep copy)
    matrix(const matrix &mat){
        nrow = mat.nrow;
        ncol = mat.ncol;
        allocateMemory();
       
        for(int i = 0; i<nrow; i++){
            for(int j = 0; j<ncol; j++){
                 data[i][j] = mat.data[i][j];
            }
        }
    }

    void allocateMemory(){
        data = new dt* [nrow];
        for(int i=0; i<nrow; i++)
            data[i] = new dt[ncol];
    }

    void setData(dt* a, int len, bool byrow = true){
        try{
            if(len<nrow*ncol && (nrow*ncol)%len != 0){
                throw "warning: data is not a sub-multiple or multiple of the number of rows.";
            }
        } catch(const char* msg){
            cerr<<msg<<endl;
        }

        if(byrow){
            for(int i = 0; i<nrow; i++){
                for(int j = 0; j<ncol; j++){
                    data[i][j] = a[(i*ncol+j)%len];
                }
            }
            cout<<endl;
        }
        else{
            for(int i = 0; i<ncol; i++){
                for(int j = 0; j<nrow; j++){
                    data[j][i] = a[(i*nrow+j)%len];
                }
            }
        }
    }

    void identity(int n){
        ncol = nrow = n;
        allocateMemory();
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++){
                if(i==j)
                    data[i][j] = 1;
                else
                    data[i][j] = 0;
            }
    }

    bool isEmpty() const{
        if(nrow)
            return false;
        return true;
    }

    //Getters
    int getnRow() const{
        return nrow;
    }

    int getnCol() const{
        return ncol;
    }

    //Setters
    void setnrow(int n){
        nrow = n;
    }
    void setncol(int n){
        ncol = n;
    }

    //Getting specifix row or col
    dt* getRow(const int index){
        dt *arr = new dt[ncol];
        for(int i=0; i<ncol; i++)
            arr[i] = data[index][i];
        return arr;
    }

    dt* getCol(const int index){
        dt *arr = new dt[nrow];
        for(int i=0; i<nrow; i++)
            arr[i] = data[i][index];
        return arr;
    }

    //removing and returning specific row of col
    dt* popRow(){
        dt *arr = getRow(ncol-1);
        nrow--;
        delete data[nrow];
        return arr;
    }

    dt* popCol(){
        dt *arr = getCol(nrow-1);
        ncol--;
        delete data[ncol];
        return arr;
    }

    //adding rows or cols
    void cbind(dt *a, int n){
            
        matrix<dt> old(nrow, ncol);
        
        for (int i = 0; i < nrow; i++) 
            old.data[i] = data[i];
        old.data = data;

        ncol++;
        allocateMemory();

        for(int i = 0; i<nrow; i++){
            for(int j = 0; j<ncol-1; j++){
                data[i][j] = old[i][j];
            }
        }
        for(int i=0; i<nrow; i++)
            data[i][ncol-1] = a[i%n];
    }

    void rbind(dt *a, int n){
        nrow++;
        data[nrow-1] = new dt[ncol];
        for(int i=0; i<ncol; i++)
            data[nrow-1][i] = a[i%n];
    }

    //removig row or col
    void removeRow(int r){
        for(int i = r; i<nrow-1; i++)
            for(int j=0; j<ncol; j++)
                data[i][j] = data[i+1][j];
        delete [] data[nrow-1];
        nrow--;
    }

    void removeCol(int c){
        for(int i = c; i<ncol-1; i++)
            for(int j=0; j<nrow; j++)
                data[j][i] = data[j][i+1];
        ncol--;
    }

    //get a specific cell value
    dt at(int i, int j){
        return data[i][j];
    }

    //Virtual Function
    //print matrix
    virtual void display() const{
        cout<<"Dimension: ["<<nrow<<", "<<ncol<<"]\n\n";
        if(nrow == 0)
            cout<<"NULL";
        for(int i = 0; i<nrow; i++){
            for(int j = 0; j<ncol; j++){
                cout<<data[i][j]<<"\t";
            }
            cout<<endl;
        }
        cout<<endl;
    }

    matrix<dt> transpose(){
        matrix<dt> mat(ncol,nrow);
        for(int i = 0; i<nrow; i++){
            for(int j=0; j<ncol; j++){
                mat.data[i][j] = data[j][i];
            }
        }
        /*matrix<dt> old(nrow, ncol);
        for (int i = 0; i < nrow; i++) 
            old.data[i] = data[i];
        old.data = data;
        nrow = old.ncol;
        ncol = old.nrow;
        allocateMemory();
        for(int i = 0; i<nrow; i++){
            for(int j=0; j<ncol; j++){
                data[i][j] = old[j][i];
            }
        }*/
        return mat;
    }

    bool isSquareMat() const{
        if(nrow!=ncol)
            return false;
        return true;
    }

    void add(matrix<dt> m1, matrix<dt> m2){
        try{
            if(typeid(dt) == typeid(char))
                throw "Matrix should be of numeric type\n";
            if(m1.nrow != m2.nrow || m1.ncol != m2.ncol){
                throw "Operands are not of same dimensions\n";
            }  
            else{
                nrow = m1.nrow;
                ncol = m1.ncol;
                allocateMemory();
                for(int i=0; i<nrow; i++)
                    for(int j=0; j<ncol; j++)
                        data[i][j] = m1[i][j] + m2[i][j];
            }   
        } catch(const char* msg){
            cerr<<msg<<endl;
        }
    }

    //Operator Overloading
    matrix<dt> operator+(matrix<dt> m){
        try{
            if(typeid(dt) == typeid(char))
                throw "Matrix should be of numeric type\n";
            if(nrow != m.nrow || ncol != m.ncol)
                throw "Non Conformable Arguments\n";
            else{
                matrix<dt> ans(nrow, ncol);
                for(int i=0; i<nrow; i++)
                    for(int j=0; j<ncol; j++)
                        ans.data[i][j] = data[i][j] + m.data[i][j];
                return ans;
            }
        }catch(const char* msg){
            cerr<<msg<<endl;
            matrix<dt> mat;
            return mat;
        }

    }

    void subt(matrix<dt> m1, matrix<dt> m2){
        try{
            if(typeid(dt) == typeid(char))
                throw "Matrix should be of numeric type\n";
            if(m1.nrow != m2.nrow || m1.ncol != m2.ncol){
                throw "Operands are not of same dimensions\n";
            }  
            else{
                nrow = m1.nrow;
                ncol = m1.ncol;
                allocateMemory();
                for(int i=0; i<nrow; i++)
                    for(int j=0; j<ncol; j++)
                        data[i][j] = m1[i][j] - m2[i][j];
            }   
        } catch(const char* msg){
            cerr<<msg<<endl;
        }
    }

    //Operator Overloading
    matrix<dt> operator-(matrix<dt> m){
       try{
            if(typeid(dt) == typeid(char))
                throw "Matrix should be of numeric type\n";
            if(nrow != m.nrow || ncol != m.ncol)
                throw "Non Conformable Arguments\n";
            else{
                matrix<dt> ans(nrow, ncol);
                for(int i=0; i<nrow; i++)
                    for(int j=0; j<ncol; j++)
                        ans.data[i][j] = data[i][j] - m.data[i][j];
                return ans;
            }
        }catch(const char* msg){
            cerr<<msg<<endl;
            matrix<dt> mat;
            return mat;
        }
    }

    void multiply(matrix<dt> m1, matrix<dt> m2){
        try{
            if(typeid(dt) == typeid(char))
                throw "Matrix should be of numeric type\n";
            if(m1.nrow != m2.nrow || m1.ncol != m2.ncol){
                throw "Operands are not of same dimensions\n";
            }  
            else{
                nrow = m1.nrow;
                ncol = m1.ncol;
                allocateMemory();
                for(int i=0; i<nrow; i++)
                    for(int j=0; j<ncol; j++)
                        data[i][j] = m1[i][j] * m2[i][j];
            }   
        } catch(const char* msg){
            cerr<<msg<<endl;
        }
    }

    //Operator Overloading
    matrix<dt> operator *(matrix<dt> m){
        try{
            if(typeid(dt) == typeid(char))
                throw "Matrix should be of numeric type\n";
            if(nrow != m.nrow || ncol != m.ncol)
                throw "Non Conformable Arguments\n";
            else{
                matrix<dt> ans(nrow, ncol);
                for(int i=0; i<nrow; i++)
                    for(int j=0; j<ncol; j++)
                        ans.data[i][j] = data[i][j] - m.data[i][j];
                return ans;
            }
        }catch(const char* msg){
            cerr<<msg<<endl;
            matrix<dt> mat;
            return mat;
        }
    }

    void product(matrix<dt> m1, matrix<dt> m2){
        try{
            if(typeid(dt) == typeid(char))
                throw "Matrix should be of numeric type\n";
            if(m1.ncol != m2.nrow)
                throw "Non Conformable Arguments\n";
            else{
                nrow = m1.nrow;
                ncol = m2.ncol;
                allocateMemory();
                dt temp;
                for(int i=0; i<nrow; i++){
                    for(int j=0; j<ncol; j++){
                        temp =0;
                        for(int k=0; k<m1.ncol; k++)
                            temp += m1.data[i][k]*m2.data[k][j];
                        data[i][j] = temp;
                    }
                }
            }
        } catch(const char* msg){
            cerr<<msg<<endl;
        }      
    }

    matrix<dt> product(matrix<dt> m){
        try{
            if(typeid(dt) == typeid(char))
                throw "Matrix should be of numeric type\n";
            if(ncol != m.nrow)
                throw "Non Conformable Arguments\n";
            else{
                matrix<dt> ans(nrow, m.ncol);
                int temp;
                for(int i=0; i<nrow; i++){
                    for(int j=0; j<m.ncol; j++){
                        temp =0;
                        for(int k=0; k<ncol; k++)
                            temp += data[i][k]*m.data[k][j];
                        ans.data[i][j] = temp;
                    }
                }
                return ans;
            }
        } catch(const char* msg){
            cerr<<msg<<endl;
            matrix<dt> mat;
            return mat;
        }      
    }

    //Operator Overloading
    dt* operator [](int i){
        try{
            if (i < nrow)
                return data[i];
            else{
                throw "Out of bound index for matrix";
            }
        } catch(const char* msg){
            cerr<<msg<<endl;
            dt *temp = NULL;
            return temp;
        }
    }

    void clear(){
        for (int i = 0; i < nrow; i++) 
            delete[] data[i];
        delete[] data;
        nrow = 0;
        ncol = 0;
    }

    //Destructor (Deep copy)
    ~matrix(){
        clear();
    }

    //Friend Function
    template <typename dtt>
    friend dtt deter(matrix<dtt> mat);

    template <typename T>
    friend matrix<float> inverse(matrix<T> &mat);

};

//Inheritance
template <typename T>
class LRmatrix : public matrix<T>{

public:
    
    LRmatrix(int n=0){
        try{
            if(typeid(T) == typeid(char))
                throw "Warning: LRmatrix can be only of numeric type.";
            else{
                this->setnrow(0);
                this->setncol(0);
            }
        } catch(const char* msg){
            cerr<<msg<<endl;
        }
    }

    LRmatrix(T *x, T *y, const int n){
        try{
            if(typeid(T) == typeid(char))
                throw "Warning: LRmatrix can be only of numeric type.";
            else{
                this->setnrow(n);
                this->setncol(2);
                this->allocateMemory();
                T *arr = new T[n*2];
                for(int i=0; i<n; i++)
                    arr[i] = x[i];
                for(int i=0; i<n; i++)
                    arr[n+i] = y[i];
                this->setData(arr, n*2, false);
            }
        }catch(const char* msg){
            cerr<<msg<<endl;
        }        
    }

    LRmatrix(T *x, const int n){
        try{
            if(typeid(T) == typeid(char))
                throw "Warning: LRmatrix can be only of numeric type.";
            else{
                this->setnrow(n);
                this->setncol(2);
                this->allocateMemory();
                
                T *arr = new T[n*2];
                for(int i=0; i<n; i++)
                    arr[i] = x[i];
                for(int i=0; i<n; i++)
                    arr[n+i] = 0;
                this->setData(arr, n*2, false);
            }
        } catch(const char* msg){
            cerr<<msg<<endl;
        }       
    }

    LRmatrix(const LRmatrix &mat){
        try{
            if(typeid(T) == typeid(char))
                throw "Warning: LRmatrix can be only of numeric type.";
            else{
                this->setnrow(mat.getnRow());
                this->setncol(mat.getnCol());
                this->allocateMemory();
                int c = mat.getnCol();
                int r = mat.getnRow();
                T *arr = new T[c*r];
                for(int i=0; i<r; i++)
                    for(int j=0; j<c; j++)
                        arr[i*c + j] = mat[i][j];
                this->setData(arr, r*c);
            }
        } catch(const char* msg){
            cerr<<msg<<endl;
        }       
    }

    struct Coefficient lm(){
        int n = this->getnRow();
        double num =0, den = 0;
        double meanX =0, meanY = 0;
        struct Coefficient ans;

        for(int i=0; i<n; i++)
        {
            meanX += this->at(i,0);
            meanY += this->at(i,1);
        }
        meanX /= n;
        meanY /= n;

        for(int i=0; i<n; i++){
            num += (this->at(i,0)-meanX)*(this->at(i,1)-meanY);
            den += (this->at(i,0)-meanX)*(this->at(i,0)-meanX);
        }

        ans.slope = num/den;
        ans.intercept = meanY - meanX*ans.slope;
        return ans;
    }

    void display(){
        cout<<"Row\tX\tY\n";
        for(int i=0; i<this->getnRow(); i++){
            cout<<i+1<<"\t"<<this->at(i,0)<<"\t"<<this->at(i,1)<<endl;
        }
        cout<<endl;
    }

    //Overriding
    void clear(){
        int n = this->getnRow();
        for (int i = 0; i < n; i++) 
            delete[] this->getRow(i);
        delete[] this;
        this->setnrow(0);
        this->setncol(0);
    }

    ~LRmatrix(){
        clear();
    }
};

template <typename dtt>
dtt deter(matrix<dtt> mat){
    try{
        if(typeid(dtt) == typeid(char))
            throw "Matrix should be of numeric type";
        if(!mat.isSquareMat())
            throw "Non Conformable Arguments";
        else{
            int n = mat.nrow;    
            dtt num1, num2, det = 1, total = 1, tmp; 
            int index;
            dtt *temp;
            temp = new dtt[n+1];        
            for (int i = 0; i < n; i++)
            {
                index = i; 
                while (index < n)
                {
                    if (mat[index][i] != 0)
                        break;
                    index++;
                }
                if (index == n) 
                    continue;
                if (index != i)
                {
                    for (int j = 0; j < n; j++){
                        tmp = mat[index][j];
                        mat[index][j] = mat[i][j];
                        mat[i][j] = tmp;
                    }
                    if((i-1)%2)
                        det *= (-1);
                }
                for (int j = 0; j < n; j++)
                    temp[j] = mat[i][j];
                for (int j = i + 1; j < n; j++)
                {
                    num1 = temp[i]; 
                    num2 = mat[j][i];
         
                    for (int k = 0; k < n; k++)
                        mat[j][k] = (num1 * mat[j][k]) - (num2 * temp[k]);

                    total = total * num1; 
                }
                
            }
            //multiplying the diagonal elements to get determinant
            for (int i = 0; i < n; i++)
                det = det * mat[i][i];
            return (det / total); // Det(kA)/k=Det(A);
        }
    } catch(const char* msg){
            cerr<<msg<<endl;
            return 0;
    }   
}

template <typename T>
matrix<float> inverse(matrix<T> &mat){
    try{
        if(typeid(T) == typeid(char))
            throw "Matrix should be of numeric type";
        if(!mat.isSquareMat())
            throw "Non Conformable Arguments";
        else{
            int n = mat.nrow;
            matrix<float> m(n);
            for(int i=0; i<n; i++)
                for(int j =0; j<n; j++)
                    m.data[i][j] = (float)mat.data[i][j];

            matrix<float> id;
            id.identity(n);

            float* temp;
            for (int i = n - 1; i > 0; i--) {
                if (m[i - 1][0] < m[i][0]) {
                    temp = m.data[i];
                    m.data[i] = m.data[i-1];
                    m.data[i-1] = temp;
                    temp = id.data[i];
                    id.data[i] = id.data[i - 1];
                    id.data[i - 1] = temp;
                }
            }

            float tmp;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) { 
                    if (j != i) { 
                        tmp = m[j][i] / m[i][i];
                        for (int k = 0; k < n; k++) { 
                            m[j][k] -= m[i][k] * tmp;
                            id[j][k] -= id[i][k] * tmp;
                        }
                    }
                }
            }

            for (int i = 0; i < n; i++) {
                tmp = m[i][i];
                for (int j = 0; j < n; j++) {
                    m[i][j] = m[i][j] / tmp;
                    id[i][j] = id[i][j] / tmp;
                }
            }
            return id;
        }
    } catch(const char* msg){
            cerr<<msg<<endl;
            matrix<float> m;
            return m;
    }      
}

//Operator Overloading
template<typename T>
ostream& operator<<(ostream& os, matrix<T> &m){
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(3);
    cout<<"Dimension: ["<<m.getnRow()<<", "<<m.getnCol()<<"]\n";
    for(int i = 0; i<m.getnRow(); i++){
        for(int j = 0; j<m.getnCol(); j++){
            cout<<m[i][j]<<"\t";
        }
        cout<<endl;
    }
    cout<<endl;
    return os;
}

ostream& operator<<(ostream& os, const struct Coefficient &cff){
    cout<<"Coefficients:\n";
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(3);
    cout<<"Intercept:  "<<cff.intercept<<endl;
    cout<<"Slope:  "<<cff.slope;
    return os;
}

//Functions
double predict(double x, const struct Coefficient &coe){
    double y;
    y = coe.intercept + x*coe.slope;
    return y;
}

template<typename T>
void predict(matrix<T> &m, const struct Coefficient &coe){
    int n = m.getnRow();
    for(int i=0; i<n; i++)
        m[i][1] = coe.intercept + m[i][0]*coe.slope;
}

matrix<int> identity(int n){
    matrix<int> m(n);
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++){
            if(i==j)
                m[i][j] = 1;
            else
                m[i][j] = 0;
        }
    return m;
}
