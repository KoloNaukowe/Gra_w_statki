
//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>





int main()
{
/*  inicjalizacja
           utworzenie interfejsu 
--- tworzenie statków i planszy [10][10] x2 - na swoje statki i na strzaly
			Abstract Class Statki
				{
				 poczatek.x
				 poczatek.y
				 orientacja
				 dlugosc
				 grafika 3D

				 ustawianie statkow
				 
				}
		    
			Class Statek.czteromasztowiec: public Statki
			    {
				 nadaj dlugosc = 4
				 wczytaj grafike = LerooooooooooooooooooooyJenkins.sexyJPG
				}
				...... etc.




			
			Class TworzeniePlansz
				{
				 utworzenie planszy gracza [10][10]
				 utworzenie planszy przeciwnika [10][10]
				 wszystkie pola oznaczone jako woda - {0}
				 wyswietlenie plansz ustawionych na pozycjach

				 ustawianie statkow gracza(drag&drop) i przeciwnika(losowo)
				}
				

--- losowanie gracza startujacego - GA

GAME LOOP(GA)
   Gracz strzela
         =sprawdz czy pole jest juz ostrzelane
		 jezeli nie:
         jezeli trafia statek to:
		   HIT LOOP (!hit)
		    =oznacz pole jako trafiony statek
			=odswiez PLANSZE
						JEZELI (KONIEC GRY) END GAME LOOP; ELSE
			=ma dodatkowy ruch
			=pole trafione zostaje oznaczone i zmienione
		    =kolejny strzal
			=sprawdz czy pole jest ostrzelane (jezeli tak to return HIT LOOP + info)
			 jezeli nie:
			   jezeli hit - return HIT LOOP
			       inaczej 
		   END HIT LOOP
         jezeli Gracz 1 nie trafia to:
		   =oznacz wode jako ostrzelana
		   =odswiez PLANSZE
		   =ruch nastepnego Gracza = return GAME LOOP(zmiana gracza)
END GAME LOOP
} 