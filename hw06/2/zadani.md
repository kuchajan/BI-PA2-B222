# Dialogové okno II

Tato úloha je rozšířením jednodušší varianty. Navíc předpokládáme zavedení prvku panel (CPanel), který umožní seskupovat ovládací prvky do skupin a s těmito skupinami pracovat.

V základní variantě úlohy existovalo okno, do kterého se umisťovaly ovládací prvky. Poloha ovládacího prvku tedy byla daná relativně k oknu. V praxi takový jednoduchý postup nestačí (např. pro okna, která mají posuvné příčky (splitter), kterými si uživatel může nastavit poměr plochy okna přidělený jednotlivým ovládacím prvkům). Chceme rozšířit naši implementaci, aby zvládala podobnou funkci. Tedy chceme vytvořit třídu CPanel, která bude schopná pojmout ovládací prvky a kterou půjde umístit do nadřazeného okna (nebo rekurentně do nadřazeného panelu). Panel se bude chovat jako speciální ovládací prvek. Jeho pozice/velikost bude definována relativně k nadřazenému oknu/panelu a velikost/poloha v něm obsažených prvků se bude přepočítávat relativně k velikosti panelu.

Testovací prostředí pro panel požaduje konstruktor (2 parametry podle ukázky), metodu add a operátor pro výstup. Rozhraní zbývajících tříd se nemění.

Správné řešení této úlohy, které projde povinnými testy na 100%, může být použito pro code review.