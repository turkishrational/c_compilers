/* =========================================================================
   TRDOS 386 TCC Portu - Hata Yakalama Test Dosyası
   ========================================================================= */

#include <stdio.h>

int main(void) {
    int normal_degisken = 10;

    /* 1. SENARYO: Syntax (Sözdizimi) Hatası - [error1 / error tetikler]
       Bilerek noktalı virgül koymuyoruz. Derleyici bir sonraki satıra 
       geçtiğinde hatayı fark edecek ve satır numarasını basmalıdır. */
    int hatali_satir = 20;

    /* 2. SENARYO: Tanımlanmamış Sembol - [error tetikler]
       Sistemde hiç var olmayan bir değişkeni çağırarak fatal error 
       ve TRDOS kalkanını (sys_exit) tetikliyoruz. */
    X_KAYNAK_DEGISKENI = 50;

    /* 3. SENARYO: Geçersiz Makro / Preprocessor Hatası - [error_noabort]
       Aşağıdaki gibi kapatılmamış veya geçersiz bir preprocessor direktifi 
       warning ya da non-aborting error üretecektir. */
    #if
    
    return 0;
}
