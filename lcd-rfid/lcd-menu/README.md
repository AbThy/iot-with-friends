# LCD Menürendszer Dokumentáció

Ez a dokumentáció egy Arduino alapú LCD menürendszer implementációját írja le, amely I2C interfészen keresztül vezérelt LCD kijelzőt és nyomógombokat használ a navigációhoz.

## Áttekintés

A program egy egyszerű menürendszert valósít meg 16x2-es LCD kijelzőn, ahol a felhasználó két gomb segítségével navigálhat a menüpontok között. A kijelző jobb oldalán egyéni karakterekből kialakított nyilak jelzik, hogy mely irányokba lehet tovább navigálni.

## Hardver követelmények

- Arduino kompatibilis lap
- 16x2 karakteres I2C LCD kijelző (0x27 címmel)
- 2 db nyomógomb
- Pull-down ellenállások a gombokhoz (1kΩ)
- Jumper kábelek

## Pin kiosztás

- LCD I2C csatlakozás:
  - SDA: A4 (Uno, Nano) / 20 (Mega)
  - SCL: A5 (Uno, Nano) / 21 (Mega)
  - VCC: 5V
  - GND: GND
- BUTTON_UP: D2 (INPUT módban)
- BUTTON_DOWN: D3 (INPUT módban)

### Gombok bekötése pull-down ellenállásokkal

A gombok pull-down ellenállásokkal vannak bekötve a következő módon:
- Gomb egyik lába: Arduino D2/D3 pin
- Gomb másik lába: 5V
- 1kΩ ellenállás a D2/D3 pin és GND között

A pull-down ellenállások használatának előnyei:
1. **Stabil alapállapot**: A pull-down ellenállás biztosítja, hogy a gomb nyugalmi állapotában (amikor nincs megnyomva) a pin biztosan LOW (0V) értéket olvasson, így elkerülhetők a lebegő bemenetből adódó hamis jelzések.
2. **Áramkorlátozás**: Megóvja az Arduino pint a túlterheléstől, hiszen a gomb megnyomásakor az ellenállás korlátozza az áramot.
3. **Zajszűrés**: Csökkenti az elektromos zaj hatását, amely téves gombnyomás-érzékelést okozhatna.
4. **Egyszerű logika**: A kód logikája egyértelmű: amikor a gomb nincs megnyomva, a digitalRead() LOW értéket ad vissza, amikor meg van nyomva, HIGH értéket.

Megjegyzés: Az 1kΩ-os ellenállás használata megfelelő a pull-down funkcióhoz, bár általában 10kΩ körüli értéket szoktak javasolni. Az 1kΩ-os ellenállás kissé nagyobb áramot enged folyni a gomb megnyomásakor, de még mindig biztonságos tartományon belül marad az Arduino számára.

## Függőségek (Könyvtárak)

- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)

## Egyéni karakterek

A program két egyéni karaktert definiál a navigációs nyilak megjelenítéséhez:

1. Felfelé mutató nyíl (upArrow):
   ```
      *
     ***
    *****
      *
      *
      *
      *
      *
   ```

2. Lefelé mutató nyíl (downArrow):
   ```
      *
      *
      *
      *
      *
    *****
     ***
      *
   ```

## Menürendszer

A menürendszer a következő funkciókat valósítja meg:

1. Előre definiált menüpontok tárolása
2. Aktuálisan kiválasztott menüpont megjelenítése
3. Navigációs lehetőségek jelzése (fel/le nyilak)
4. Gombnyomásra a menüben való mozgás

A példában három menüpont van definiálva:
- "1.Fast&Furious"
- "2.Fast2Furious"
- "3.TokyoDrift"

## Funkciók részletezése

### setup()

- Inicializálja az LCD kijelzőt
- Beállítja a háttérvilágítást
- Elindítja a soros kommunikációt (9600 baud)
- Beállítja a gombok pinjeit INPUT módba
- Létrehozza az egyéni karaktereket
- Megjeleníti az első menüpontot

### loop()

- Beolvassa a gombok állapotát
- A BUTTON_UP gomb megnyomására növeli a kiválasztott menüpont indexét (ha lehetséges)
- A BUTTON_DOWN gomb megnyomására csökkenti a kiválasztott menüpont indexét (ha lehetséges)
- Késleltetést alkalmaz (250ms) a gombok túl gyors érzékelésének elkerülésére
- Frissíti a kijelzőt az aktuális menüponttal

### updateMenuOption(int selectedIndex)

- Frissíti a kijelzőt az aktuálisan kiválasztott menüponttal
- Elkerüli a felesleges frissítéseket, ha a kiválasztott menüpont nem változott
- Megjeleníti a navigációs nyilakat a lehetséges irányok függvényében

### printArrows(bool possibleUp, bool possibleDown)

- Megjeleníti a felfelé mutató nyilat, ha felfelé lehet navigálni
- Megjeleníti a lefelé mutató nyilat, ha lefelé lehet navigálni

## Változók

- `menuItems[]`: Tárolja a menüpontok szövegeit
- `menuItemCount`: A menüpontok száma
- `selectedMenuIndex`: Az aktuálisan kiválasztott menüpont indexe
- `lastRenderedMenuItem`: Az utoljára megjelenített menüpont indexe
- `buttonUpState` és `buttonDownState`: A gombok aktuális állapota
- `isStartedAtNow`: Jelzi, hogy első indításról van-e szó

## Bővítési lehetőségek

A kód könnyen bővíthető a következőkkel:

1. További menüpontok hozzáadása (a `menuItems` tömb bővítésével)
2. Almenük létrehozása
3. Menüpontok kiválasztásának kezelése (pl. egy harmadik "Select" gombbal)
4. Menüpontokhoz rendelt funkciók végrehajtása
5. EEPROM használata a beállítások mentéséhez

## Hibaelhárítás

- Ha az LCD nem működik, ellenőrizd az I2C címet (alapértelmezetten 0x27, de lehet 0x3F is)
- Ellenőrizd a gombok bekötését és működését külön tesztprogrammal
- Ha a navigáció nem működik megbízhatóan, növeld a késleltetési időt a loop() függvényben
- Ha a gombok néha véletlenszerűen működnek, ellenőrizd a pull-down ellenállásokat