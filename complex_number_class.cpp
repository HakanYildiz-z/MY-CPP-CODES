//#################################################################################################################################
//       KOMPLEKS SAYILARDA İŞLEMLEMLER : kompleks sayılarda doğrudan bildiğimiz matematikteki gibi işlem yapamayız. x ekseni ve y ekseni gib düşün.
//#################################################################################################################################

#include<iostream>

class complex {
private:
    double real , imag ;    // burada double olan iki farklı değişken tanımlıyoruz bunların değer atamalarını aşağıda yapacağız şimdilik bunlar sadece birer çekirdek.

public:
    complex(double r, double i) : real(r) , imag (i){}

    complex operator+(const complex& other) const {    // complex türünden operator+ adında bir fonksiyon yaratıyoruz. bunu sadece + yazarak kullanıyoruz.                                                      // dışardan 2 şer tane real ve imag alıcaz ve bunları kendi içinde topluyacağız.
                                                       // dışarıdan 2 tane real ve imag değer verilecek bunları kendi içinde topluyacaz bu fonksiyon ile.
        return complex(real + other.real, imag + other.imag);
    }

    complex operator-( const complex& other) const {
        return complex(real - other.real, imag - other.imag );
    }

    complex operator*(const complex& other) const {      // kompleks sayılarda çarpma yapıyoruz. burası tamamen matematikten ibaret.
        return complex(real * other.real - imag * other.imag, real * other.imag + imag * other.real );
    }

    complex operator/(const complex& other) const {      // kompleks sayılarda bölme işlemidir ayrıca sıfır kontrolü de yapıyoruz.
        double denominator = other.real * other.real + other.imag * other.imag;
        if ( denominator == 0.0 ) return complex(0, 0);
        return complex((real * other.real + imag * other.imag) / denominator,(imag * other.real - real * other.imag) / denominator);
    }

    // dost fonksiyon yardımıyla private ye erişip onun içindeki bizim seçtiğimiz elemanları cout ile main içinde çıktı verilebilmesine olanak tanıyoruz.
    // vuradaki os ostream üzerinden türediği için cout ile aynı kafada zaten biz burada ostreamın içine girip onu düzenliyoruz aynı + fonksiyonumuza yaptığımız gibi.
    // (c.imag >= 0 ? "+" : "-")  bu bir if else bloğudur ? işareti true olursa yani >= ise soldaki değer soru işareti false olursa sağdaki değer yazılır.
    // std::abs(c.imag)  içine verdiğimiz değeri pozitife çevirir. bunu yapıyoruz çünkü if kontrolü ile orada işareti zaten belirledik.


    friend std::ostream& operator << (std::ostream& os , const complex& c) {
        os << c.real << (c.imag >= 0 ? " +" : " -") << std::abs(c.imag) << " i";
        return os;
    }

};
int main() {

    // içindeki fonksiyonlar ile matematiksel işlemleri yapan sınıfların içine vrileri veriyoruz
    complex c1 (5.0, 9.0);
    complex c2 (2.0,7.0);

    std::cout << "Toplam: " << c1 + c2 << std::endl;   // cout ile bildiğimiz çıktıları veriyoruz.
    std::cout << "Fark:   " << c1 - c2 << std::endl;
    std::cout << "Carpim: " << c1 * c2 << std::endl;
    std::cout << "Bolum:  " << c1 / c2 << std::endl;
    return 0;
}
