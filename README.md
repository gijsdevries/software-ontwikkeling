User manual softwareontwikkeling
Projectgroep en studentengegevens
Projectgroep 2:
-	Piotr van Houwelingen, 1861692 (Notulist, voorzitter vergaderingen)
-	Gijs de Vries, 1860901 (Git master)
-	Luuk Laseur, 1820771 (Scrum leader)

3-lagen model
Zoals beschreven in het cursusmateriaal van het vak softwareontwikkeling moet er professionele software ontwikkeld worden via een meerdere lagen systeem. In dit geval is het type software-architectuur wat hiervoor gebruikt wordt het 3-lagenmodel, dit is op de canvas pagina van de cursus met de volgende afbeelding weergegeven, zie figuur 1. Het 3-lagen model houdt in dat er een 1e en 3e laag zijn die gescheiden worden door een 2e laag. In de door ons gemaakte toepassing is dit model als volgt ingericht:
-	De frontlayer
-	De logiclayer
-	De I/O-layer

<img width="945" height="308" alt="image" src="https://github.com/user-attachments/assets/0c874061-57c0-43ed-be31-29f7b5e99ee5" />

 
Figuur 1: Overzicht 3-lagenmodel
Frontlayer
De frontlayer is de laag waarin de gebruiker communiceert met de applicatie en dus zijn commando’s kan doorgeven. Deze commando’s worden via UART ontvangen, verwerkt en gecheckt. Hierbij wordt bij een juist commando de instructie doorgegeven aan de logiclayer. Bij een onjuist commando wordt er op basis van het probleem een error bericht terug gestuurd naar de gebruiker. Ook regelt de laag wanneer een commando ontvangen kan worden door gebruik te maken van een flag, op deze manier wordt gezorgd dat de applicatie niet overbelast raakt.
Logiclayer
In de logiclayer wordt het verwerkte commando uitgevoerd. Hier wordt dus bepaalt welke pixels welke kleur moeten worden. Vervolgens wordt dit naar de I/O-layer doorgestuurd.
I/O-layer
De I/O-layer verandert de kleur van de pixels op het VGA-scherm op basis van de bepaalde pixel kleuren van de logiclayer.
Graphical design
Op basis van het eerder beschreven 3-lagenmodel en de gegeven basis eisen voor de applicatie is een high level design opgesteld, zie figuur 2: 

Figuur 2: High level design


In dit ontwerp staan de volgend (hoofd)functies centraal:
Frontlayer:
-	USART2_BUFFER(): Verwerkt de data uit de USART2 ontvangstbuffer en voegt ze toe aan een dynamische commando-buffer. Wanneer een newline-karakter wordt gedetecteerd, wordt het volledige commando verwerkt door Buffer_check().
-	Buffer_Check (): Vergelijkt het eerste woord in de buffer met een lijst van bekende commando's. Als een match wordt gevonden, wordt de bijbehorende functie  Buffer_to_struct() aangeroepen om het commando verder te verwerken. Anders wordt een foutmelding verzonden.
-	Buffer_to_struct(): Afhankelijk van het commando worden de argumenten uit de buffer gehaald, geconverteerd naar de juiste datatypes, en opgeslagen in een struct. Na validatie wordt de bijbehorende tekenfunctie in de logiclayer aangeroepen.
Logiclayer: 
-	clearScreenToVGA(): Deze functie maakt elke pixel dezelfde van tevoren gekozen kleur.
-	lineToVGA(): Tekent een lijn op het VGA-scherm met een bepaalde dikte.
-	rectangleToVGA(): Tekent een rechthoek op het VGA-scherm.
-	textToVGA(): Tekent gegeven tekst op het VGA-scherm.
-	bitmapToVGA(): Tekent een bitmap op het VGA-scherm.
I/O-layer: 
-	UB_VGA_SetPixel(): Stelt één pixel in op het scherm met een specifieke kleur.
 

Commando’s
Hieronder zal worden aangegeven welke functies beschikbaar zijn en hoe deze functies kunnen gebruikt worden.
Bitmap
Commando: bitmap, nr, x-lup, y-lup
-	nr: Het nummer van de bitmap (deze zijn vooraf vastgesteld).
-	x-lup, y-lup: De coördinaten van de linkerbovenhoek vanaf waar de bitmap wordt getekend.
Lijn
Commando: lijn, x1, y1, x2, y2, kleur, dikte
-	x1, y1: De coördinaten van het beginpunt van de lijn.
-	x2, y2: De coördinaten van het eindpunt van de lijn.
-	kleur: Kleur van de lijn.
-	dikte: De dikte van de lijn.
Rechthoek
Commando: rechthoek, x_lup, y_lup, breedte, hoogte, kleur, gevuld
-	x_lup, y_lup: De coördinaten van de linkerbovenhoek vanaf waar de rechthoek wordt getekend.
-	breedte: De breedte van de rechthoek.
-	hoogte: De hoogte van de rechthoek.
-	kleur: De kleur van de rechthoek.
-	gevuld: Geeft aan of de rechthoek gevuld moet zijn (0 = niet gevuld, 1 = gevuld).
Tekst
Commando: tekst, x, y, kleur, tekst, fontnaam, fontgrootte, fontstijl
-	x, y: De coördinaten van de linkerbovenhoek vanaf waar de tekst wordt afgebeeld.kleur: Kleur van de tekst.
-	tekst: De tekst die moet worden afgebeeld.
-	fontnaam: De naam het lettertype (Consolas, Arial).
-	fontgrootte: De grootte van het lettertype (1 voor klein, 2 voor groot).
-	fontstijl: De stijl van het lettertype (normaal, vet, cursief).
Clearscherm
Commando: clearscherm, kleur
-	kleur: Kleur van het scherm

