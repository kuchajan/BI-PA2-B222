# Výsledková listina

Úkolem je realizovat šablonu třídy, která dokáže sestavovat výsledkové listiny v závislosti na zadaných vstupech - výsledcích zápasů dvojic soupeřů.

Předpokládáme, že máme několik soupeřů. V jednom zápase se utká dvojice soupeřů, výsledek zápasu je nějak popsán (například booleovskou hodnotou, dvojicí počtu bodů, dvojicí časů, ...). Protože chceme mít třídu obecnou, bude výsledek zápasu reprezentován jako generický datový typ. Takto bude do třídy vloženo několik výsledků pro zadané dvojice soupeřů (soupeři jsou jednoznačně identifikováni jménem - řetězcem). Následně chceme zjistit, zda na základě zadaných výsledků lze sestavit jednoznačnou výsledkovou listinu a případně tuto listinu i vypočítat. Trikem je, že při sestavování výsledku chceme vycházet pouze z informací o vítězi/poraženém ze zadaných zápasů.

Výsledková listina je určena jednoznačně, pokud pro každého soupeře Xi na pozici i platí:
- Xi porazil všechny soupeře Xj, i+1 ≤ j ≤ n. Soupeře porazil buď přímo, nebo zprostředkovaně (Xi porazil Xa, Xa porazil Xb, ..., Xz porazil Xj, i+1 ≤ a < b < ... < j ≤ n),
- Xi nevyhrál ani neremizoval se žádným soupeřem Xk, 0 ≤ k ≤ i-1.

Vezměme příklad, kdy bylo zadáno, že A porazí B a B porazí C. Pak je zřejmě správná výsledková listina A B C. Pokud by ale bylo zadáno, že:
- A porazí B
- B porazí C
- C porazí A

případně:
- A porazí B
- A porazí C

případně:

- A porazí B
- B remizuje s C

pak výsledkovou listinu nelze jednoznačně určit (museli bychom dodat informace o dalších zápasech nebo pořadí stanovit i na základě dalších kritérií, to v této úloze nebudeme dělat).

Bylo zmíněno, že zápas je zadaný jako trojice (soupeř1, soupeř2, výsledek), kde výsledek je nějaký datový typ popisující například skóre, časy nebo cokoliv jiného. Výsledek je potřeba převést na informaci o vítězi/remíze/poraženém, to bude mít na starosti porovnávač (komparátor) předaný metodě pro určení výsledkové listiny. Porovnávač je funkce, funktor nebo lambda-výraz, který pro daný výsledek vrací návratovou hodnotu:
- zápornou, pokud soupeř1 prohrál,
- nulovou, pokud soupeř1 a soupeř2 remizovali,
- kladnou, pokud soupeř1 vyhrál.

## CContest
Celkové rozhraní šablony třídy CContest bude:
- šablona je parametrizovaná generickým parametrem M_
    - tento typ popisuje výsledek zápasu. Pro datový typ M_ je garantováno, že je přesouvatelný, kopírovatelný a zrušitelný (CopyConstructibe, MoveConstructible, CopyAssignable, MoveAssignable a Destructible). Další vlastnosti typu nejsou garantované, speciálně, není garantované, že typ M_ má implicitní konstruktor (nemusí být DefaultConstructible).
- implicitní konstruktor
    - vytvoří prázdnou instanci CContest,
- addMatch(contestant1, contestant2, result)
    - metoda přidá výsledek mezi soupeřem contestant1 a contestant2. Soupeři jsou zadaní v podobě řetězců. Výsledek result je datového typu M_. Metoda si uloží informaci o tomto zápasu do nějakého úložiště ve své instanci. Volání addMatch lze řetězit. Metoda vyvolá výjimku std::logic_error, pokud byl výsledek zápasu mezi contestant1 a contestant2 již dříve zadán.
- isOrdered (comparator)
    - metoda rozhodne, zda vyplněné výsledky zápasů vedou k jednoznačné výsledkové listině nebo ne. Návratovou hodnotou je true pokud je výsledková listina jednoznačná, false pokud ne. Parametrem volání je comparator - funkce, funktor nebo lambda výraz, který bude volán při získávání výsledku zápasu (převede M_ na záporné číslo / nulu / kladné číslo podle popisu výše). Metoda nesmí modifikovat instanci CContest a nesmí házet výjimky.
- results (comparator)
    -metoda vypočte výsledkovou listinu. Výsledkem je STL seznam (std::list) řetězců se jmény soupeřů v pořadí od prvního (celkového vítěze) k poslednímu. Pokud nelze výsledkovou listinu sestavit (není jednoznačná), metoda vyhodí výjimku std::logic_error. Parametr comparator má stejný význam jako u metody isOrdered.

## Odevzdání
Odevzdávejte zdrojový kód s implementací šablony třídy CContest. Za základ implementace použijte přiložený zdrojový kód. Pokud v kódu ponecháte bloky podmíněného překladu, lze takový zdrojový kód lokálně testovat a zároveň jej odevzdávat Progtestu.

Hodnocení je rozděleno na povinnou a bonusovou část. V povinné části se testují instance s malým počtem soupeřů a zápasů. Pro jeho úspěšné zvládnutí stačí základní algoritmus pracující v čase počet_soupeřů x počet_zápasů. Pro zvládnutí bonusového testu je potřeba použít algoritmus efektivnější.

Pro základní řešení se může hodit vyhledání všech přímo nebo zprostředkovaně poražených soupeřů. Toto dokáže algoritmus BFS (prohledávání do šířky).

Do parametry vyhozené výjimky zadejte nějaký popis problému (např. "duplicate match"). Fakticky se ale text ve vyhozené výjimce nekontroluje.
