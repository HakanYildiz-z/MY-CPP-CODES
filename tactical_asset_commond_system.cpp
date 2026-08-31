#include<iostream>      // giriş çıkış kütüphanesi
#include<string>        // metin dizileri için
#include<vector>        // geniş kapsamlı diziler için
#include<algorithm>     // std::remo_if , std::sort ve çok daha fazla düzenleyici fonksiyona sahiptir.
#include<utility>       // std::move  kullanımı ve birkaç başka fonksiyona izin verir.
#include<iomanip>       // çıktı manipülsyonları ve yerleşimi için kullanılır.
#include<limits>        //  std::numeric_limits<std::streamsize>::max()   kodunu kullanmamızı sağlıyor std::cin içinde temizlik yaparken kullanıyoruz bunu

class asset {      // varlık sınıfını oluşturuyoruz.
private:
    std::string name;
    int id;
public:

    asset(std::string name ,int id) : name(name), id(id) {}

    // burada yazdığımız fonksiyon dışarı true veya false dönderiyor. referans yardımıyla asset türünde other nesnesi oluşturuyoruz.
    // varlık1 < varlık2 şeklinde karşılaştırma yapacaksak varlık1 i this temsil ediyor ve varlık2 yi için ise other.id temsil görevini üztleniyor.
    // varlık1 in içindeki id < varlık2 nin içindeki id ise true değeri ortaya çıkar değilse false olur. buradaki varlık1-2 asset türünde nesnelerdir.
    bool operator<(const asset& other) const {
        return this -> id < other.id;
    }
    bool operator==(int targetid) const {  // içeriye int ile oluşturduğumuz yeni veriyi veriyoruz. soldakini asset nesnesi sağdaki ise ona yeni ataacağımız değer.
        return this -> id == targetid;
    }

  // get ile kapsülleme yapıyoruz.
    int get_id()const{ return id;}
    const std::string& get_name()const{ return name;}    //stringleri referans ile almak daha verimlidir. hiç kopyalama işine girmez ve hız korunur.
};

class asset_manager {    // varlık yönetim sınıfını oluşturuyoruz.
private:
    std::vector<asset> asset_collocation;  // varlık koleksiyonu vector ünü oluşturyoruz.

    // asset_maneger x ; asset_manager y ; asset_manager z; şeklinde 3 nesne oluşturduğumuzda bu nesnelerin içindeki total_search_operations elemanına ulaşmak istiyoruz
    // x.totalSearchOperations y.totalSearchOperations z.totalSearchOperations elemanı static int olarak değilde sadece int olarak yazılırsa 3 farklı elemana sahip oluruz.
    static int total_search_operations ;    // sayaç olduğu için her yerde aynı olmalı
    bool is_sorted;                         // verilerin sırasının yapılı mı yoksa bozuk mu olduğunu sorar. yeni eleman eklenince veya çıkartılınca sıra bozulur ve yeniden düzenlenmesi gerekir.

public:
    // kurucuyu yazıyoruz fakat kurucunun içine static int total_search_operations   elemanını veremeyiz static elemanlar kurucu içinde yer alamaz.
    // vector elemanları doğrudan hafıza bağlı olduğu için kurucuya yazmasak da sorun teşkil etmez.
    asset_manager( ):is_sorted(true) {}    // yönetim sınıfı olduğu için kurucu dışarıdan parametre almak zorunda değil.

    void add_asset( int id , std::string name) {   // varlık ekliyeceğiz. doğrudan hafıza ekleneceği için dışarıya return değerinin dönmesine gerek yok o yüzden void.
        asset_collocation.emplace_back(std::move (name) , id);    // emplace_back fonksiyonun içindeki sıralama asset sınıfındaki gibi olmalı cpp buna özellikle dikkat eder.
        is_sorted = false;      // yeni eleman eklendiğinde sıralama bozulur.
        std::cout << "varlik sisteme basari ile eklendi \n";
    }
    void remove_asset(int id) {    // hafızadan varlığı tamamen izolo edip silen fonksiyon.
        auto it = std::remove_if(asset_collocation.begin(), asset_collocation.end() , [id] (const asset& asset){return asset.get_id() == id;});

        if (it != asset_collocation.end()) {                                        // sona ulaşmadıysa yani it bitmediyse diye sorguluyor. it bitmediyse şunu yap.
            asset_collocation.erase(it , asset_collocation.end());    // it içinde hem başlangıç hemde bitişle birlikte olan kapsama alanı var.
        std::cout << "varlik silme islemi basri ile yapildi.\n";                     // biz yine de erase istiyor diye bitişi tekrardan yazıyoruz. neden ?? çünkü erase öyle istiyor.
        }else {
        std::cout << "varlik sisteme islemi basarisiz. \n";
        }
    }
    void prepare_for_search() {
        if (is_sorted) {
            std::cout << "varliklar zaten duzende. \n";
            return;          // ek maliyetten en hızlı kaçmak için fonksiyonu sonlandırıyoruz. ve std::sort fonksiyonuna falan hiç girmiyor. onları else içine alabilirdik fakat ek maliyet gereksiz.
        }
        std::sort (asset_collocation.begin() , asset_collocation.end());   // sıralama yapıyoruz sort aynı remove_if gibi algorithm kütüphanesine aittir.
        is_sorted = true;                                                          // sıralamayı yaptıktan sonra sıralama parametresini true yani sıralama tamam şeklinde değiştiriyoruz.
        std::cout << "siralama yapildi";
    }
    void search_asset(int id) {      // varlık aradığımız bir fonksiyon id değerini içine ver o sana varlığı bulsun
        if (!is_sorted) {      // sıralı olup olmadığını kontrol ediyor.
            std::cout << "Envanter duzenli degil.  \n";
            return;
        }
        total_search_operations++;               // static elemanı bir artırıyoruz. pivate elemanlarının değerlerini kendi public fonksiyonları değiştirebilir. çünkü kendi kasalarında yer alıyor.
        asset dummy_target("", id);      // asset türünde belirli bir varlık oluşturyoruz yandaki parantez olmasa belirli olmazdı. isimsiz ve sadece id taşıyan sahte bir hedef.

        // std::binary_search arama operatörüdür vektör adresinin begin ve end sınırları içinde dummy_target verisini alır ve getirir.
        if (std::binary_search(asset_collocation.begin() , asset_collocation.end() , dummy_target)) {
            std::cout << id << " hedef basari ile tespit edildi \n  ";
        }else {
            std::cout << id << "sistemde yok \n";
        }
    }
    void display_assets() const {               // varlıkları görüntüleyeceğimiz bir fonksiyon.  içine parametre atamıyoruz ve değişiklik yapmıyoruz tek amacımız varlıkları görüntülemek.
        if (asset_collocation.empty()) {
            std::cout << "varlik yok \n" ;
            return;                             // buradaki return döngülerdeki break ile aynı işlevi görür.
        }
        std::cout << "\n VARLIK RAPORU \n ";

        // const_iterator vector içinde gömülü bir elemandır. değiştirlimezdir sadece veri yazdırmak için kullanılır.
        // const_iterator olarak it oluşturyoruz ve bunu asset_collocation vectorünün başına sabitliyoruz. it son elemana eşit olmadığı sürece bunu devam ettiriyoruz ve ++ ile her döngüde bir eleman sağa kaydırıyoruz.
        for( std::vector<asset>::const_iterator it = asset_collocation.begin() ; it != asset_collocation.end(); ++it) {
            std::cout << "ID :"<<std::left << std::setw (5) << it -> get_id() << "  |  NAME :" << std::left << std::setw(30) << it -> get_name();        // burada get_id() finksiyonuna (*it).get_id şeklinde de erişebiliriz.
        }
    }
    static int get_total_search_operations() {       // sayı dış dünyadan görüntüleyebilmek için get fonksiyonu.
        return total_search_operations;             // static değişkenlerin get fonksiyonlarında consy koyamıyoruz.
    }
};

int asset_manager::total_search_operations = 0;     // static değişkenlerine ilk değeri bu şekilde atamak zorundayız sonrasında atama yapamayız private koruması etkili olur.

void clear_input_stream() {                         // cin arka planını temizlemek için kullanıcağımız fonksiyonu yazıyoruz. içine girdi parametresi koymamıza gerek yok.
    std::cin.clear();                               // cin veri alacağında beklediği türde veri verilmezse kilitlenir. int istiyorsa int verilmelidir... clear ise bu kiliti açar.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main () {
    asset_manager assets;
    int choice = 0 ;

    while (true) {
        std::cout << "\n----KONTROL PANELI----\n";
        std::cout << "1. Yeni Varlik Ekle \n";
        std::cout << "2. Varlik Imha Et \n";
        std::cout << "3. Birlikleri Sirala \n";
        std::cout << "4. Hedef Ara \n";
        std::cout << "5. Envanter Raporunu Gor \n";
        std::cout << "6. Operasyon Istatistikleri \n";
        std::cout << "7. Sistemi Kapat \n";
        std::cout << "\n Emriniz :";

        if(!(std::cin >> choice)){                  // burada cin istendiği gibi alınmadıysa if bloğu devreye girer.
            if (std::cin.eof()) break;              // eğer akış kesildiği için cin içine düştüysek problem yok der kirlilik yok der ve break yapar.
            clear_input_stream();                   // eğer akış kesilme değilde hatalı veri girildiği için if içine düştüysek temizlik şart.
            std::cout << "hata olustu sadece sayi girisi verin. \n " ;
        }
        if (choice == 7) {
            std::cout << "Sistemden cikiliyor...\n";
            break;
        }

        switch (choice) {        // BURADAKİ CASE ELEMANLARININ İÇİNDE KENDİ YAZDIĞIMIZ FONKSİYONLARI KULLANICAZ.

            case 1: {       // asset ekleme
                int id;
                std::string name;
                std::cout << " \n eklenecek olan varligin id sini giriniz :";
                if (!(std::cin >> id)) {            // veri girişi hatalı olduysa akış kesilmesi veya yanlış tür girişi.
                    if (std::cin.eof()) break;      // eğer akış kesildiyse sorun yok çöp veri girişi olmamıştır.
                    clear_input_stream();           // akış kesilmesi yerine yanlış tür girişi olduysa burada temizlik şarttır.
                    std::cout << "\n yanlis veri girisi oldu lutfen id degerini sadece sayi olarak girin. ";
                    continue;
                }
                clear_input_stream();
                std::cout << "\n varlik ismini giriniz: ";
                if (!std::getline(std::cin,name)) {
                    if (std::cin.eof()) break;
                    clear_input_stream();
                    std::cout << "\n yanlis veri girisi oldu lutfen kontrol edip tekrar deneyin : ";
                    continue;
                }
                assets.add_asset(id,name);
                break;
            }
            case 2: {       // asset silme
                int id;
                std::cout << " \n silinecek  olan varligin id sini giriniz :";
                if (!(std::cin >> id)) {            // veri girişi hatalı olduysa akış kesilmesi veya yanlış tür girişi.
                    if (std::cin.eof()) break;      // eğer akış kesildiyse sorun yok çöp veri girişi olmamıştır.
                    clear_input_stream();           // akış kesilmesi yerine yanlış tür girişi olduysa burada temizlik şarttır.
                    std::cout << "\n yanlis veri girisi oldu lutfen id degerini sadece sayi olarak girin. ";
                    continue;
                }
                assets.remove_asset(id);
                break;
            }
            case 3: {       // asset sıralama
                assets.prepare_for_search();
                break;
            }
            case 4: {
                int id;
                std::cout << " \n aranacak olan varligin id sini giriniz :";
                if (!(std::cin >> id)) {            // veri girişi hatalı olduysa akış kesilmesi veya yanlış tür girişi.
                    if (std::cin.eof()) break;      // eğer akış kesildiyse sorun yok çöp veri girişi olmamıştır.
                    clear_input_stream();           // akış kesilmesi yerine yanlış tür girişi olduysa burada temizlik şarttır.
                    std::cout << "\n yanlis veri girisi oldu lutfen id degerini sadece sayi olarak girin. ";
                    continue;
                }
                assets.search_asset(id);
                break;
            }
            case 5: {
                assets.display_assets();
                break;
            }
            case 6: {
                std::cout << "\n toplam gerceklestirilen arama :" << asset_manager::get_total_search_operations();
                break;

            }
        }
    }
}
