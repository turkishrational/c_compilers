@echo off
echo ===================================================================
echo [TRDOS-386 MASTER OBJECT LINKING VIA OPEN WATCOM WLINK]
echo ===================================================================

:: wlink'e parametre dosyasýný .txt uzantýsýyla paslýyoruz
wlink @tcc_link.txt

echo TCC.PRG milimetrik relocation zýrhý ile doðrudan flat olarak uretildi!
