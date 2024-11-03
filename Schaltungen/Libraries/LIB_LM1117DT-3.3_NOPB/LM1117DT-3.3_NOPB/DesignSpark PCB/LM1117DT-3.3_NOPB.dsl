SamacSys ECAD Model
155129/620563/2.49/3/3/Integrated Circuit

DESIGNSPARK_INTERMEDIATE_ASCII

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r175_100"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 1) (shapeHeight 1.75))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(padStyleDef "r635_555"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 5.55) (shapeHeight 6.35))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Default"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 50 mils)
			(strokeWidth 5 mils)
		)
	)
	(patternDef "TO228P991X238-3N" (originalName "TO228P991X238-3N")
		(multiLayer
			(pad (padNum 1) (padStyleRef r175_100) (pt -4.7, 2.285) (rotation 90))
			(pad (padNum 2) (padStyleRef r175_100) (pt -4.7, -2.285) (rotation 90))
			(pad (padNum 3) (padStyleRef r635_555) (pt 2.4, 0) (rotation 90))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0, 0) (textStyleRef "Default") (isVisible True))
		)
		(layerContents (layerNumRef 30)
			(line (pt -2.082 -3.365) (pt 4.138 -3.365) (width 0.001))
		)
		(layerContents (layerNumRef 30)
			(line (pt 4.138 -3.365) (pt 4.138 3.365) (width 0.001))
		)
		(layerContents (layerNumRef 30)
			(line (pt 4.138 3.365) (pt -2.082 3.365) (width 0.001))
		)
		(layerContents (layerNumRef 30)
			(line (pt -2.082 3.365) (pt -2.082 -3.365) (width 0.001))
		)
		(layerContents (layerNumRef 30)
			(line (pt -5.85 -3.65) (pt 5.85 -3.65) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 5.85 -3.65) (pt 5.85 3.65) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 5.85 3.65) (pt -5.85 3.65) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt -5.85 3.65) (pt -5.85 -3.65) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(arc (pt 0, 0) (radius 0.35) (startAngle 0.0) (sweepAngle 0.0) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(arc (pt 0, 0) (radius 0.35) (startAngle 180.0) (sweepAngle 180.0) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 0 -0.5) (pt 0 0.5) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt -0.5 0) (pt 0.5 0) (width 0.05))
		)
		(layerContents (layerNumRef 28)
			(line (pt -2.02 -3.27) (pt 4.075 -3.27) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 4.075 -3.27) (pt 4.075 3.27) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 4.075 3.27) (pt -2.02 3.27) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt -2.02 3.27) (pt -2.02 -3.27) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 4.075 -2.605) (pt 4.365 -2.605) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 4.365 -2.605) (pt 4.955 -1.952) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 4.955 -1.952) (pt 4.955 1.952) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 4.955 1.952) (pt 4.365 2.605) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 4.365 2.605) (pt 4.075 2.605) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1.15 -3.25) (pt -3.45 -3.25) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -3.45 -3.25) (pt -3.45 3.25) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -3.45 3.25) (pt -1.15 3.25) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -5.3, 3.3) (radius 0.125) (startAngle 0.0) (sweepAngle 0.0) (width 0.25))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -5.3, 3.3) (radius 0.125) (startAngle 180.0) (sweepAngle 180.0) (width 0.25))
		)
	)
	(symbolDef "LM1117DT-3_3_NOPB" (originalName "LM1117DT-3_3_NOPB")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 230 mils -25 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 2) (pt 0 mils -100 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 230 mils -125 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 3) (pt 1300 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 1070 mils -25 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(line (pt 200 mils 100 mils) (pt 1100 mils 100 mils) (width 6 mils))
		(line (pt 1100 mils 100 mils) (pt 1100 mils -200 mils) (width 6 mils))
		(line (pt 1100 mils -200 mils) (pt 200 mils -200 mils) (width 6 mils))
		(line (pt 200 mils -200 mils) (pt 200 mils 100 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 1150 mils 300 mils) (justify Left) (isVisible True) (textStyleRef "Default"))

	)
	(compDef "LM1117DT-3.3_NOPB" (originalName "LM1117DT-3.3_NOPB") (compHeader (numPins 3) (numParts 1) (refDesPrefix IC)
		)
		(compPin "1" (pinName "ADJ/GND") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "3" (pinName "INPUT") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "4" (pinName "VOUT") (partNum 1) (symPinNum 3) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "LM1117DT-3_3_NOPB"))
		(attachedPattern (patternNum 1) (patternName "TO228P991X238-3N")
			(numPads 3)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "3")
				(padNum 3) (compPinRef "4")
			)
		)
		(attr "Manufacturer_Name" "Texas Instruments")
		(attr "Manufacturer_Part_Number" "LM1117DT-3.3/NOPB")
		(attr "Mouser Part Number" "926-LM1117DT-3.3NOPB")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Texas-Instruments/LM1117DT-33-NOPB?qs=X1J7HmVL2ZHOT670myqy2w%3D%3D")
		(attr "Arrow Part Number" "LM1117DT-3.3/NOPB")
		(attr "Arrow Price/Stock" "https://www.arrow.com/en/products/lm1117dt-3.3nopb/texas-instruments")
		(attr "Description" "Space saving 800-mA low-dropout linear regulator with internal current limit")
		(attr "Datasheet Link" "http://docs-emea.rs-online.com/webdocs/0780/0900766b807800cc.pdf")
		(attr "Height" "2.38 mm")
	)

)
