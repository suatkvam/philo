izin verilen fonksiyonlar
### 1. Bellek Yönetimi (Memory)

* **`malloc`**: (Memory Allocation) Hafızadan (heap) belirli bir boyutta yer ayırmanızı sağlar. Filozoflarınızın veya mutex'lerinizin (çatalların) bilgilerini tutmak için `struct`'lara yer ayırırken bunu kullanırsınız.
* **`free`**: `malloc` ile ayırdığınız hafıza alanını sisteme geri verir. Bunu yapmazsanız "memory leak" (hafıza sızıntısı) oluşur.
* **`memset`**: Bir bellek bloğunu (örneğin `malloc` ile yeni ayırdığınız bir alanı) belirlediğiniz bir karakterle (genellikle `0` ile) doldurur. Değişkenlerinizi "temizlemek" veya sıfırlamak için kullanılır.

---

### 2. Girdi/Çıktı (I/O)

* **`printf`**: Ekrana formatlı metin yazdırmanızı sağlar. Projede "philosopher X is eating" (filozof X yemek yiyor) gibi durum loglarını basmak için bunu kullanacaksınız.
* **`write`**: `printf`'e göre daha düşük seviyeli bir yazdırma fonksiyonudur. Bu da ekrana veri yazar, ancak genellikle `printf` kadar kullanışlı değildir (formatlama yapmaz).

---

### 3. Zaman Fonksiyonları (Time)

* **`gettimeofday`**: Mevcut zamanı mikrosaniye (saniyenin milyonda biri) hassasiyetinde almanızı sağlar. Projedeki zaman damgalarını (`timestamp_in_ms`) ve bir filozofun en son ne zaman yemek yediğini (`time_to_die`) hesaplamak için bu şarttır.
* **`usleep`**: Programınızı (veya daha doğrusu *mevcut thread'i*) mikrosaniye cinsinden belirttiğiniz süre kadar "uyutur" (durdurur). Filozofun `time_to_eat` (yemek yeme süresi) ve `time_to_sleep` (uyuma süresi) kadar beklemesini sağlamak için kullanılır.

---

### 4. Thread (İş Parçacığı) Fonksiyonları

Bu fonksiyonlar, programınızın aynı anda birden fazla iş yapmasını (paralel çalışmasını) sağlar. Projede her filozof ayrı bir thread olmalıdır.

* **`pthread_create`**: Yeni bir thread (iş parçacığı) oluşturur ve başlatır. Her bir filozof için bunu bir kez çağırırsınız.
* **`pthread_join`**: Ana thread'in (genellikle `main` fonksiyonunuzun), başka bir thread'in (örneğin bir filozofun) işini bitirmesini beklemesini sağlar.
* **`pthread_detach`**: Bir thread'i "bağımsız" (detached) hale getirir. Bu, o thread bittiğinde kaynaklarının otomatik olarak serbest bırakılacağı anlamına gelir. `pthread_join` kullanmayacaksanız bunu kullanabilirsiniz.

---

### 5. Mutex (Senkronizasyon) Fonksiyonları

Mutex (Mutual Exclusion), "karşılıklı dışlama" anlamına gelir. Aynı anda sadece bir thread'in belirli bir kod bölümüne veya kaynağa (projede **çatallar**) erişebilmesini sağlayan bir **kilit** mekanizmasıdır. Bu, "data race" (veri yarışı) olmasını engeller.

* **`pthread_mutex_init`**: Bir mutex değişkenini kullanıma hazırlar (ilklendirir). Projede her çatal (fork) için bir mutex başlatmanız gerekecek.
* **`pthread_mutex_destroy`**: `init` ile başlattığınız mutex'i yok eder. Program biterken kaynakları temizlemek için kullanılır.
* **`pthread_mutex_lock`**: Mutex'i **kilitlemeye** çalışır.
    * Eğer mutex kilitli değilse (çatal boştaysa), thread kilidi alır ve yoluna devam eder.
    * Eğer mutex zaten başka bir thread tarafından kilitlenmişse (çatal kullanımdaysa), bu thread kilit açılana kadar burada **bekler**.
* **`pthread_mutex_unlock`**: Kilitlediğiniz mutex'in kilidini **açar**. Filozof çatalı kullanmayı bitirdiğinde, kilidi açarak diğer filozofların o çatalı alabilmesine izin verir.

---

### Data Race (Veri Yarışı) Nedir?

"Data race" (veri yarışı), bu projenin kalbidir ve bu fonksiyonlar da o kalbi koruyan mekanizmalardır.

Basit bir dille **veri yarışı**, birden fazla thread'in (filozofun) aynı anda *aynı* hafıza alanına (çatala) erişmeye çalışması ve en az birinin bu veriyi *değiştirmeye* (çatalı almaya) çalışması durumudur.

Programınızın çalışmasını bir dizi talimat olarak düşünün. İşletim sistemi, bu talimatları thread'ler arasında çok hızlı bir şekilde değiştirerek (context switch) hepsinin aynı anda çalışıyormuş gibi görünmesini sağlar.

**Mutex Olmadan Sorun (Data Race):**

İki filozof (Thread A ve Thread B) aynı anda **sol çatala** (Fork 1) uzanmak istesin.

1.  **Thread A (Filozof 1):** Çatal 1'in durumunu kontrol eder. `(if (fork_available == TRUE))`
2.  **Thread A:** Çatalın "müsait" (TRUE) olduğunu görür.
3.  **CONTEXT SWITCH!** (İşletim sistemi aniden Thread A'yı durdurur ve Thread B'ye geçer. Thread A daha çatalı "meşgul" (FALSE) olarak *işaretleyemedi*.)
4.  **Thread B (Filozof 2):** Çatal 1'in durumunu kontrol eder. `(if (fork_available == TRUE))`
5.  **Thread B:** O da çatalın "müsait" (TRUE) olduğunu görür (çünkü Thread A onu henüz alamadı).
6.  **Thread B:** Çatalı alır ve durumunu `fork_available = FALSE` yapar.
7.  **CONTEXT SWITCH!** (İşletim sistemi Thread B'yi durdurur ve Thread A'ya döner.)
8.  **Thread A:** 3. adımda kaldığı yerden devam eder. O da çatalın "müsait" olduğunu *düşünüyordu*, bu yüzden çatalı alır ve durumunu `fork_available = FALSE` yapar.

**Sonuç:** İki filozof da aynı çatalı aldığını sanıyor. Bu, projenin "filozofların çatal kopyalamasını önleme" kuralını ihlal eder ve programın çökmesine veya yanlış çalışmasına neden olur.

---

### Çözüm: Fonksiyonlar Nasıl Birlikte Çalışır (Mutex)

İşte `pthread_mutex` fonksiyonları burada devreye girer. Mutex, bir "kilit"tir. Veri yarışını önlemek için bu "kritik bölgeyi" (çatalı kontrol etme ve alma işlemini) koruruz.

Bir çatalı artık sadece bir `int` (müsait/değil) olarak değil, bir **oda** olarak düşünün. Çatal bu odanın içinde.

* **`pthread_mutex_init`**: Odayı ve kapısındaki kilidi inşa eder. (Her çatal için bir tane).
* **`pthread_mutex_lock`**: Filozofun çatalı almak için odanın kapısını **kilitlemeye** çalışmasıdır.
* **`pthread_mutex_unlock`**: Filozofun çatalı bırakıp odanın kilidini **açmasıdır**.

**Mutex ile Doğru Senaryo:**

1.  **Thread A (Filozof 1):** Çatal 1'i almak ister. `pthread_mutex_lock(&fork1_mutex)` çağrısını yapar.
2.  **Thread A:** Kilit boştadır. Kilidi alır (kapıyı kilitler) ve odaya girer.
3.  **Thread A:** Odanın içindeki çatalın durumuna bakar (`fork_available == TRUE`).
4.  **CONTEXT SWITCH!** (Thread A durdurulur, *ama kilit hala ondadır*).
5.  **Thread B (Filozof 2):** Çatal 1'i almak ister. `pthread_mutex_lock(&fork1_mutex)` çağrısını yapar.
6.  **Thread B:** Kapının (mutex'in) kilitli olduğunu görür. **İçeri giremez.** İşletim sistemi Thread B'yi, kilit açılana kadar burada *uykuya alır* (bloke eder).
7.  **CONTEXT SWITCH!** (Thread A'ya geri dönülür).
8.  **Thread A:** Kaldığı yerden devam eder. Çatalı alır (`fork_available = FALSE`) ve odadan çıkar.
9.  **Thread A:** `pthread_mutex_unlock(&fork1_mutex)` çağrısını yapar (kapının kilidini açar).
10. **Sistem:** Kilit açıldığı için, 6. adımda bekleyen **Thread B**'yi uyandırır.
11. **Thread B:** Artık kilidi alabilir (kapıyı kilitler) ve odaya girer. Çatalın durumuna bakar (`fork_available == FALSE`) ve çatalın meşgul olduğunu görerek geri çıkar (veya diğer çatalı bekler).

**Sonuç:** Veri yarışı engellendi. Aynı anda sadece bir filozof o çatal üzerinde işlem yapabildi.

### Diğer Fonksiyonların Rolü

* **`pthread_create`**: Bu senaryodaki her bir "Thread A" ve "Thread B"yi (yani filozofları) başlatan fonksiyondur.
* **`gettimeofday`**: Filozofun `time_to_die` süresini kontrol etmek için "şu anki zaman" ile "son yediği zaman" arasındaki farkı hesaplamanızı sağlar.
* **`usleep`**: Filozofun kilitleri *tuttuğu* (`time_to_eat`) veya *bıraktığı* (`time_to_sleep`) sırada simülasyonu gerçekçi kılmak için bekleten fonksiyondur.
