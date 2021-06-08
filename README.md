# LPRS2_MAX1000_Game_Console_Emulator

# IMPORTANT TO READ ALL OF THIS

# Pokretanje igre: 

Preuzeti .zip arhivu i raspakovati. Pozicionirati se u odgovarajuci direktorijum (direktorijum sa izvornim kodom project.c) iz konzole (linux terminal or mingw32 for win).
Pokrenuti komande ./waf prerequisites i ./waf configure. Nakon toga, moguce je pokrenuti projekat komandom ./waf build run --app=project. Ukoliko dodje do greske da ne moze da se otvori biblioteka "sprites_rgb333.h", otvoriti projekat anim_state.c i zakomentarisati #include te datoteke. Nakon toga ponovo pokrenuti program i igra moze da pocne.

#Kako se igra igra:

Kada se igra pokrene, nalazite se na pocetnom ekranu igre. Ptica krece da se pomera pritiskom na desnu strelicu na tastaturi (START), nakon toga je moguce kontrolisati pticu tasterima gore i dole (up/down). Ukoliko dodje do kolizije ptice i prepreka, igru je moguce zapoceti ponovo pritiskom na taster "A", ili izaci iz igre pritiskom na taster "levo" (leva strelica). 

# Autori:
  #    Marija Kedžić
  #    Jovana Jezdimirović
  #    Radenko Mihajlović
      
 Izvorni kod je javno dostupan i dozvoljeno je svako koriscenje i modifikovanje prilozenog koda u cilju unapredjenja igre. Podrzavamo buduci rad studenata na ovom projektu i nadamo se da ce uciniti igru jos zanimljivijom i boljom.
 
 # NAPOMENA: !!! Nemojte preuzeti kod i okaciti kao svoje resenje bez ulozenog truda da poboljsate performanse igre, ucinite je zanimljivijom i otklonite eventualne nedostatke !!! 
