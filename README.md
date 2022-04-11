# BE_Cpp

Bureau d'études de conception orientée objet en C++

Alban Brunet
Alexis Pierre dit lambert

Avril 2022

Une girouette qui détecte la force et la direction du vent
Plusieurs cercles de diodes qui s'allume pour indiquer les infos.
Affiche les infos sur un écran LCD.
Un actionneur :	une pome à eau, qui pompe d'un réservoir bas vers haut

Mets les mesures dans une liste,
	Si plusieurs mesures élevées d'affilées,
	allume pompe

Exception : un bouton pressé déclanche un exception et met le programme en veille

_______

Capteurs :
	un petit aimant (2mm de diamètre)
	un capteur à effet Hall pou mesurer la vitese de rotation
	un codeur numérique pour mesurer la position du mat
	une petite pompe électrique pilotable
	un écran LCD I2C
	un bouton

	Optionnel :
	( un démultipléxeur
	 des diodes (x16)  )

Pris dans l'armoire :
	- écran LCD : Grove-LCD RGB Backlight
	- DC/AC Current Senor
	- Tx et Rx infrarouge
	



