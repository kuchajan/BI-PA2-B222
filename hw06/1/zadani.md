# Dialogové okno I
Úkolem je navrhnout a implementovat sadu tříd, které budou simulovat prvky v uživatelském rozhraní. Konkrétně jde o okno (CWindow), tlačítko (CButton), textový vstup (CInput), textový popisek (CLabel) a combobox (CComboBox).

Úkol je zaměřen na návrh tříd, kde bude využito dědičnosti, polymorfismu a abstraktních metod. Pokud jsou tyto OOP prostředky použité racionálně, není implementace příliš dlouhá. Naopak, pokud provedete návrh špatně, bude se Vám kód opakovat a implementační soubor bude velký.

Navržené třídy budou GUI pouze simulovat. Nebudou nic vykreslovat, budou pouze vytvářet modely uživatelských prvků. Třídy tedy půjde použít k sestavení modelu uživatelských ovládacích prvků v okně, půjde manipulovat s jejich stavem a půjde zobrazit jejich stav ve výstupním proudu (viz níže). Jednotliv8 okna a jednotlivé uživatelské prvky půjde identifikovat pomocí jejich číselného id, pokud toto id je unikátní, pak pomocí tohoto id lze prvek i vyhledat. Pokud id unikátní není, bude ovládací prvek fungovat, ale nepůjde jej podle tohoto ID vyhledat.

Okno bude reagovat na změnu umístění/velikosti. Poloha a velikost jednotlivých ovládacích prvků je zadána relativně vzhledem k šířce/výšce nadřazeného okna (velikost/poloha je tedy desetinné číslo v rozsahu 0 až 1). Při změně velikosti okna se přepočtou pozice a velikosti jednotlivých ovládacích prvků. Při přepočtu se vynásobí odpovídající šířka (výška) desetinným číslem polohy/velikosti a pro zobrazení se použije takto vzniklé desetinné číslo.

## Třídy

Rozhraní tříd pro reprezentaci ovládacích prvků je velmi podobné. Konstruktor typicky má 2 nebo 3 parametry. Prvním parametrem bývá celočíselné id přiřazené ovládacímu prvku. Následuje informace o umístění (CRect se 4 složkami typu desetinné číslo, které udávají relativní pozici x, y a relativní velikosti w a h). Posledním parametrem bývá řetězec, který určuje zobrazovaný obsah. Dále, všechny třídy implementující ovládací prvky musí být schopné se zobrazit ve výstupním proudu operátorem << a musí být schopné se správně kopírovat (hluboké kopie). Testovací prostředí většinou požaduje pouze tuto základní funkcionalitu (tj. konstruktor, kopírování a operátor pro výstup). Vaše implementace ale zřejmě bude požadovat rozhraní další, které si do tříd budete muset doplnit.

### CButton
Reprezentuje tlačítko. Testovací prostředí požaduje pouze konstruktor a operátor pro výstup.

### CInput
Reprezentuje textový vstup. Posledním parametrem konstruktoru je řetězec, který je ve vstupním boxu zobrazován. Testovací prostředí požaduje konstruktor, operátor pro výstup a další dvě metody:
- setValue ( x ), která nastaví obsah na řetězec x,
- getValue ( ), která vrátí aktuálně zobrazovaný řetězec.

### CLabel
Reprezentuje statický text. Posledním parametrem konstruktoru je zobrazovaný řetězec. Testovací prostředí požaduje konstruktor a operátor pro výstup.

### CComboBox
Reprezentuje combobox. Testovací prostředí požaduje konstruktor, operátor pro výstup a další 3 metody:
- add ( x ), která přidá zadaný řetězec x do seznamu nabídek,
- setSelected ( x ), která nastaví nabídku s indexem x jako vybranou (výchozí je vybraná nabídka s indexem 0),
- getSelected (), která zjistí index aktuálně vybrané nabídky.

### CWindow
Reprezentuje okno. Třída má následující rozhraní:
- konstruktor CWindow(id, title, absPos), který vytvoří okno s názvem title. Okno bude mít nastavenou pozici x,y a velikost w x h podle zadaného parametru absPos. Pozor, zde jsou rozměry absolutní.
- Metoda add( x ), která přidá ovládací prvek x.
- Metoda setPosition ( absPos ), která změní pozici/velikost okna. Zadané hodnoty jsou absolutní souřadnice a velikosti okna po změně. Metoda zároveň zajistí přepočet pozice a velikosti všech ovládacích prvků obsažených v okně.
- Metoda search ( id ) vyhledá ovládací prvek podle zadaného id. Pokud prvek se zadaným id v okně neexistuje, metoda vrátí nullptr. Pokud v okně existuje více ovládacích prvků se stejným id, metoda vrátí odkaz na některý z nich (např. na první).
- Operátor << zobrazí obsah okna do výstupního proudu, formát je zřejmý z přiloženého zdrojového kódu.

### CRect
Pomocná třída reprezentující obdélník. Tato třída je implementovaná v testovacím prostředí a je zahrnutá i do přiloženého zdrojového souboru. Při odevzdávání ji ponechte v bloku podmíněného překladu.

## Odevzdání

Odevzdávejte zdrojový kód s implementací tříd CWindow, CButton, CInput, CLabel a CComboBox. Do odevzdávaného souboru zahrňte všechny potřebné podpůrné deklarace. Části vkládání hlaviček a Vaše testy ponechte v bloku podmíněného překladu, jak je naznačeno v přiložené ukázce.

## Poznámky

- Používejte operátory pro přetypování (dynamic_cast) s rozmyslem. V ukázce byl použit, zde mu zcela zabránit nelze. Ale vlastní referenční implementace v sobě nemá žádné přetypování ani žádné použití RTTI. Obecně, RTTI, dynamic_cast a typeid vedou k více větvenému kódu, který je hůře čitelný a hůře rozšiřitelný. Navrhněte třídy tak, abyste si vystačili s polymorfismem.
- Vaše řešení musí používat třídy, třídy musí tvořit hierarchii, dědičnost a polymorfismus musí být použité. V této úloze je použití dědění a polymorfismu vhodné, navíc, testovací prostředí odmítne řešení, které by dědění, polymorfismus a dynamicky vázané metody nevyužívalo (takové řešení bude odmítnuto na chybě při kompilaci).
- Výstup je ve formě "stromu", byť v této úloze je strom nejvýše dvojúrovňový. Číslo v hranatých závorkách udává id okna/ovládacího prvku. V kulatých závorkách jsou pak absolutní souřadnice (x,y,w,h).
- Výpis obsahuje přepočtené absolutní souřadnice ovládacích prvků. Absolutní souřadnice jsou známé až po vložení ovládacího prvku do nějakého okna. Testovací prostředí proto nezkouší vypisovat vytvořené objekty ovládacích prvků před jejich vložením do nějakého okna.
- Řešení této úlohy nelze použít pro code review. Lze použít až správné řešení rozšířené verze (Dialogové okno II).
