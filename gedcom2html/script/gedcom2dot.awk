#! /usr/bin/awk

function initTemplates() {
    MALE_COLOR_BG    = "blue"
    MALE_COLOR_FG    = "white"
    MALE_GLYPH       = "♂"

    FEMALE_COLOR_BG  = "pink"
    FEMALE_COLOR_FG  = "black"
    FEMALE_GLYPH     = "♀"

    UNKNOWN_COLOR_BG = "black"
    UNKNOWN_COLOR_FG = "white"
    UNKNOWN_GLYPH    = "?"

    COUPLE_COLOR_BG  = "white"
    COUPLE_COLOR_FG  = "black"
    COUPLE_GLYPH_UNION            = "×"
    COUPLE_GLYPH_UNION_RELIGIOUS  = "×r"
    COUPLE_GLYPH_UNION_SEPARATION = "÷"

    CUT_GLYPH_ASC  = "✂ ✂ ✂ ✂ ✂"
    CUT_GLYPH_DESC = CUT_GLYPH_ASC

    BIRTH_GLYPH     = "°"
    BAPTISM_GLYPH   = "b"
    DEATH_GLYPH     = "+"
    BURIAL_GLYPH    = "s"
    STILLBORN_DEATH = "°/+"
}

function initMacros() {
    SEX_MALE       = 1
    SEX_FEMALE     = 2
    SEX_UNKNOWN    = 10
    SEP            = "#"
    GRAPH_NAME     = "genealogy"
}

function initVars() {
    tabIndi[""]    = 0
    tabFam[""]     = 0
    elt0           = ""
}

function getHead(list) {
    head = list
    gsub(/ .*$/, "", head)
    return head
}

function getTail(list) {
    tail = list
    gsub(/^[^ ]* ?/, "", tail)
    return tail
}

function printStdErr(s) {
    print s > "/dev/stderr"
}

function checkParameters() {
    FILTER_ORDER   = toupper(FILTER_ORDER)
    FILTER_INDI    = toupper(FILTER_INDI)
    FILTER_DEPTH   = toupper(FILTER_DEPTH)
    FILTER_FRATRIE = toupper(FILTER_FRATRIE)
    FILTER_MODE    = toupper(FILTER_MODE)
    gsub(/ +/, " ", FILTER_INDI)
    gsub(/ /, ".*", FILTER_INDI)
    if (FILTER_ORDER != "DESC" && FILTER_ORDER != "ASC") {
        FILTER_ORDER = "NO"
    }
    if (FILTER_DEPTH ~ /^[0-9]+$/) {
    } else {
        FILTER_DEPTH = -1
    }
    if (FILTER_DEPTH == 0) {
        FILTER_DEPTH = -1
    }
    if (FILTER_FRATRIE != "YES") {
        FILTER_FRATRIE = "NO"
    }
    if (FILTER_MODE != "COGNATIC" && FILTER_MODE != "AGNATIC") {
        FILTER_MODE = "NO"
    }
    printStdErr("FILTER_ORDER . = '" FILTER_ORDER   "'")
    printStdErr("FILTER_INDI .. = '" FILTER_INDI    "'")
    printStdErr("FILTER_DEPTH . = '" FILTER_DEPTH   "'")
    printStdErr("FILTER_FRATRIE = '" FILTER_FRATRIE "'")
    printStdErr("FILTER_MODE .. = '" FILTER_MODE    "'")
    printStdErr("TITLE ........ = '" TITLE          "'")
}

function addLineToDotOutput(line) {
    if (dotOutput != "") {
        dotOutput = dotOutput "\n"
    }
    dotOutput = dotOutput line
}

# TODO : Between two dates (or from ... to ...) ?
# TODO : Republic year with token "an" ?
function transcoDate(gedcomDate) {
    readableDate = ""
    v = ""
    split(gedcomDate, tabDate, " ")
    dayFlag = 0
    month = ""
    monthFlag = 0
    for (i in tabDate) {
        #printStdErr("")
        #printStdErr("i=" i "=")
        oldV = v
        v = tabDate[i]
        #printStdErr("v=" v "=")
        if (dayFlag == 0 && monthFlag == 0 && v ~ /^[0-9]+$/) {
            if (length(v) == 1) {
                v = "0" v
            }
            dayFlag = 1
            #printStdErr("day=" day "=")
            #printStdErr("dayFlag=" dayFlag "=")
        }
        
        # Dates prefixes
        if (v == "BEF") {
            readableDate = readableDate " <i>avant </i>"
        } else if (v == "AFT") {
            readableDate = readableDate " <i>après </i>"
        } else if (v == "BET") {
            readableDate = readableDate " <i>entre </i>"
        } else if (v == "ABT") {
            readableDate = readableDate " <i>vers </i>"
        } else if (v == "CAL") {
            readableDate = readableDate " <i>calculée </i>"
        } else if (v == "EST") {
            readableDate = readableDate " <i>estimée </i>"
        
        # Gregorian months
        } else if (v == "JAN") {
            month = "01"
            monthFlag = 1
        } else if (v == "FEB") {
            month = "02"
            monthFlag = 1
        } else if (v == "MAR") {
            month = "03"
            monthFlag = 1
        } else if (v == "APR") {
            month = "04"
            monthFlag = 1
        } else if (v == "MAY") {
            month = "05"
            monthFlag = 1
        } else if (v == "JUN") {
            month = "06"
            monthFlag = 1
        } else if (v == "JUL") {
            month = "07"
            monthFlag = 1
        } else if (v == "AUG") {
            month = "08"
            monthFlag = 1
        } else if (v == "SEP") {
            month = "09"
            monthFlag = 1
        } else if (v == "OCT") {
            month = "10"
            monthFlag = 1
        } else if (v == "NOV") {
            month = "11"
            monthFlag = 1
        } else if (v == "DEC") {
            month = "12"
            monthFlag = 1
        
        # French republic months
        } else if (v == "VEND") {
            readableDate = readableDate " vendémiaire"
        } else if (v == "BRUM") {
            readableDate = readableDate " brumaire"
        } else if (v == "FRIM") {
            readableDate = readableDate " frimaire"
        } else if (v == "NIVO") {
            readableDate = readableDate " nivose"
        } else if (v == "PLUV") {
            readableDate = readableDate " pluviose"
        } else if (v == "VENT") {
            readableDate = readableDate " ventose"
        } else if (v == "GERM") {
            readableDate = readableDate " germinal"
        } else if (v == "FLOR") {
            readableDate = readableDate " floréal"
        } else if (v == "PRAI") {
            readableDate = readableDate " prairial"
        } else if (v == "MESS") {
            readableDate = readableDate " messidor"
        } else if (v == "THER") {
            readableDate = readableDate " thermidor"
        } else if (v == "FRUC") {
            readableDate = readableDate " fructidoc"
        } else if (v == "COMP") {
            readableDate = readableDate " jour complémentaire"
        
        # Hebrew months
        } else if (v == "TSH") {
            readableDate = readableDate " tishri"
        } else if (v == "CSH") {
            readableDate = readableDate " cheshvan"
        } else if (v == "KSL") {
            readableDate = readableDate " kislev"
        } else if (v == "TVT") {
            readableDate = readableDate " tevet"
        } else if (v == "SHV") {
            readableDate = readableDate " shevat"
        } else if (v == "ADR") {
            readableDate = readableDate " adar"
        } else if (v == "ADS") {
            readableDate = readableDate " adar sheni"
        } else if (v == "NSN") {
            readableDate = readableDate " nisan"
        } else if (v == "IYR") {
            readableDate = readableDate " iyar"
        } else if (v == "SVN") {
            readableDate = readableDate " sivan"
        } else if (v == "TMZ") {
            readableDate = readableDate " tammuz"
        } else if (v == "AAV") {
            readableDate = readableDate " av"
        } else if (v == "ELL") {
            readableDate = readableDate " elul"
        
        # Other tokens
        } else {
            #printStdErr("month=" month "=")
            #printStdErr("monthFlag=" monthFlag "=")
            if (monthFlag == 0) {
                readableDate = readableDate " " v
            } else {
                if (flagDay == 1) {
                    readableDate = readableDate "/"
                }
                readableDate = readableDate "/" month "/" v
                month     = ""
                monthFlag = 0
            }
        }
        #printStdErr("readableDate=" readableDate "=")
    }
    gsub(/^ +/,  "",  readableDate)
    gsub(/ +$/,  "",  readableDate)
    gsub(/  */,  " ", readableDate)
    gsub(/^ +$/, "",  readableDate)
    return(readableDate)
}

function capitalize(s) {
    return toupper(substr(s,1,1)) tolower(substr(s,2));
}

function transcoOccu(s) {
    if (toupper(s) ~ /\(SANS\)/) {
        s = ""
    }
    return(s)
}

function transcoName(idIndi) {
    if (tabIndi_sex[idIndi] == SEX_MALE) {
        symbol  = MALE_GLYPH
    } else if (tabIndi_sex[idIndi] == SEX_FEMALE) {
        symbol  = FEMALE_GLYPH
    } else {
        symbol  = UNKNOWN_GLYPH
    }
    firstName = tabIndi_name[idIndi]
    lastName  = tabIndi_name[idIndi]
    gsub(/\/.*$/,    "", firstName)
    gsub(/^[^/]*\//, "", lastName)
    gsub(/\/$/,      "", lastName)
    if (toupper(firstName) ~ /INCONNU/ || toupper(lastName) ~ /INCONNU/) {
        firstName = ""
        lastName  = CUT_GLYPH_ASC
    } if (toupper(firstName) ~ /\(SANS\)|MORT-NÉ/) {
        firstName = "<i>sans prénom</i>"
    }
    if (tabIndi_stillBorn[idIndi] == 1) {
        stillBornLabel = " (mort-né)"
    } else {
        stillBornLabel = ""
    }
    return symbol " " lastName " " firstName stillBornLabel
}

function transcoPlace(place) {
    gsub(/^ +/,  "", place)
    gsub(/ +$/,  "", place)
    gsub(/,+$/,  "", place)
    gsub(/^ +$/, "", place)
    if (toupper(place) ~ /,FRANCE(,|$)/) {
        split(place, places, ",")
        if (length(places) == 5) {
            place = places[1] "," places[3]
        } else if (length(places) == 6) {
            place = places[1] "/" places[6] "," places[3]
        }

        
    }
    if (place != "") {
        place = " (" place ")"
    }
    return place
}

function addIndiToDotOutput(idIndi) {
    if (idIndi != "" && tabIndi_printFlag[idIndi] == 0) {
        tabIndi_printFlag[idIndi] = 1
        indiShape = "Mrecord"
        if (tabIndi_sex[idIndi] == SEX_MALE) {
            bgcolor = MALE_COLOR_BG
            fgcolor = MALE_COLOR_FG
        } else if (tabIndi_sex[idIndi] == SEX_FEMALE) {
            bgcolor = FEMALE_COLOR_BG
            fgcolor = FEMALE_COLOR_FG
        } else {
            bgcolor = UNKNOWN_COLOR_BG
            fgcolor = UNKNOWN_COLOR_FG
        }
        indiLabelBegin = ""
        indiLabelId    = ""
        indiLabelOccu  = ""
        indiLabelBirt  = ""
        indiLabelChr   = ""
        indiLabelDeat  = ""
        indiLabelBuri  = ""
        indiLabelUls   = ""
        indilabelEnd   = ""
        indiLabel      = ""
        indiOccu       = transcoOccu(tabIndi_occu[idIndi])
        indiLabelBegin = "<table border=\"0\" cellborder=\"0\" cellpadding=\"3\" bgcolor=\"white\">"
        indiLabelId    = "<tr><td border=\"0\" bgcolor=\"" bgcolor "\" align=\"center\"><font color=\"" fgcolor "\">" transcoName(idIndi) "</font></td></tr>"
        if (indiOccu != "") {
            indiLabelOccu = "<tr><td align=\"left\">" indiOccu "</td></tr>"
        }
        if (tabIndi_stillBorn[idIndi] == 1) {
            if (tabIndi_birtDate[idIndi] != "" || tabIndi_birtPlace[idIndi] != "") {
                indiLabelBirt = "<tr><td align=\"left\">" STILLBORN_DEATH " " transcoDate(tabIndi_birtDate[idIndi]) transcoPlace(tabIndi_birtPlace[idIndi]) "</td></tr>"
            }
        } else {
            if (tabIndi_birtDate[idIndi] != "" || tabIndi_birtPlace[idIndi] != "") {
                indiLabelBirt = "<tr><td align=\"left\">" BIRTH_GLYPH " " transcoDate(tabIndi_birtDate[idIndi]) transcoPlace(tabIndi_birtPlace[idIndi]) "</td></tr>"
            }
            if (tabIndi_chrDate[idIndi] != "" || tabIndi_chrPlace[idIndi] != "") {
                indiLabelChr = "<tr><td align=\"left\">" BAPTISM_GLYPH " " transcoDate(tabIndi_chrDate[idIndi]) transcoPlace(tabIndi_chrPlace[idIndi]) "</td></tr>"
            }
            if (tabIndi_deatDate[idIndi] != "" || tabIndi_deatPlace[idIndi] != "") {
                indiLabelDeat = "<tr><td align=\"left\">" DEATH_GLYPH " " transcoDate(tabIndi_deatDate[idIndi]) transcoPlace(tabIndi_deatPlace[idIndi]) "</td></tr>"
            }
            if (tabIndi_buriDate[idIndi] != "" || tabIndi_buriPlace[idIndi] != "") {
                indiLabelBuri = "<tr><td align=\"left\">" BURIAL_GLYPH " " transcoDate(tabIndi_buriDate[idIndi]) transcoPlace(tabIndi_buriPlace[idIndi]) "</td></tr>"
            }
        }
        if (tabIndi_descCut[idIndi] == 1) {
            indiLabelUls = "<tr><td align=\"center\">" CUT_GLYPH_DESC "</td></tr>"
        }
        indilabelEnd = "</table>"
        indiLabel = "<" indiLabelBegin indiLabelId indiLabelOccu indiLabelBirt indiLabelChr indiLabelDeat indiLabelBuri indiLabelUls indilabelEnd ">"
        addLineToDotOutput("  \"INDI" idIndi "\" [shape=" indiShape ",label=" indiLabel ",regular=true,style=filled,fillcolor=white];")
    }
}

#
# Add an union to output
#
function addUnionToDotOutput(idCouple) {
    if (idCouple != "" && tabCouple_printFlag[idCouple] == 0) {
        tabCouple_printFlag[idCouple] = 1
        famShape   = "Mrecord"        
        bgcolor    = COUPLE_COLOR_BG
        fgcolor    = COUPLE_COLOR_FG
        labelUnion = "<i>union</i>"
        if (tabFam_marrUst[idCouple] == "MARRIED") {
            if (   tabFam_marrDate[idCouple]    != "" || tabFam_marrPlace[idCouple]    != "" \
                || tabFam_marrRelDate[idCouple] != "" || tabFam_marrRelPlace[idCouple] != "" \
                || tabFam_divDate[idCouple]     != "" || tabFam_divPlace[idCouple]     != "") {
                labelUnion = "Mariage"
                if (tabFam_divDate[idCouple] != "" || tabFam_divPlace[idCouple] != "") {
                    labelUnion = labelUnion " et divorce"
                }
            }
        } else if (tabFam_marrUst[idCouple] == "COHABITATION") {
            labelUnion = "Union libre"
            if (tabFam_divDate[idCouple] != "" || tabFam_divPlace[idCouple] != "") {
                labelUnion = labelUnion " et séparation"
            }
        }
        famLabelBegin = "<table border=\"0\" cellborder=\"0\" cellpadding=\"3\" bgcolor=\"white\">"
        famLabelId    = "<tr><td bgcolor=\"" bgcolor "\" align=\"center\"><font color=\"" fgcolor "\">" labelUnion "</font></td></tr>"
        if (tabFam_marrDate[idCouple] != "" || tabFam_marrPlace[idCouple] != "") {
            famLabelMarr = "<tr><td align=\"left\">" COUPLE_GLYPH_UNION " " transcoDate(tabFam_marrDate[idCouple]) transcoPlace(tabFam_marrPlace[idCouple]) "</td></tr>"
        } else {
            famLabelMarr = ""
        }
        if (tabFam_marrRelDate[idCouple] != "" || tabFam_marrRelPlace[idCouple] != "") {
            famLabelMarrRel = "<tr><td align=\"left\">" COUPLE_GLYPH_UNION_RELIGIOUS " " transcoDate(tabFam_marrRelDate[idCouple]) transcoPlace(tabFam_marrRelPlace[idCouple]) "</td></tr>"
        } else {
            famLabelMarrRel = ""
        }
        if (tabFam_divDate[idCouple] != "" || tabFam_divPlace[idCouple] != "") {
            famLabelDiv = "<tr><td align=\"left\">" COUPLE_GLYPH_UNION_SEPARATION " " transcoDate(tabFam_divDate[idCouple]) transcoPlace(tabFam_divPlace[idCouple]) "</td></tr>"
        } else {
            famLabelDiv = ""
        }
        famlabelEnd = "</table>"
        famLabel = "<" famLabelBegin famLabelId famLabelMarr famLabelMarrRel famLabelDiv famlabelEnd ">"
        addLineToDotOutput("  \"FAM" idCouple "\" [shape=" famShape ",label=" famLabel ",regular=true,style=filled,fillcolor=white];")
    }
}

#
# General link
#
function addLinkToDotOutput(idFromType, idFrom, idToType, idTo, weight) {
    if (tabLink_printFlag["LINK_" idFromType "_" idFrom "_" idToType "_" idTo] == 0) {
        tabLink_printFlag["LINK_" idFromType "_" idFrom "_" idToType "_" idTo] = 1
        addLineToDotOutput("\n" "  \"" idFromType idFrom "\" -> \"" idToType idTo "\" [dir=none,weight=" weight "];")
    }
}

#
# Link from individual to family
#
function addLinkIndiFamToDotOutput(idFromIndi, idToFam) {
    addLinkToDotOutput("INDI", idFromIndi, "FAM", idToFam, 1)
}

#
# Link from family to individual
#
function addLinkFamIndiToDotOutput(idFromFam, idToIndi) {
    addLinkToDotOutput("FAM", idFromFam, "INDI", idToIndi, 2)
}

#
# Print Union, husband and wife
#
function addCoupleHusbandAndWifeToDotOutput(idFamOutput) {
    if (idFamOutput != "" && tabFam_printFlag[idFamOutput] == 0) {
        tabFam_printFlag[idFamOutput] = 1
        addUnionToDotOutput(idFamOutput)
        if (tabFam_idHusb[idFamOutput] != "") {
            if (FILTER_MODE != "COGNATIC") {
                addLinkIndiFamToDotOutput(tabFam_idHusb[idFamOutput], idFamOutput)
            }
        }
        if (tabFam_idWife[idFamOutput] != "") {
            if (FILTER_MODE != "AGNATIC") {
                addLinkIndiFamToDotOutput(tabFam_idWife[idFamOutput], idFamOutput)
            }
        }
    }
}

#
# Print Union, husband and wife
#
function addFamilyToDotOutput(idFamOutput) {
    if (idCouple != "" && tabCouple_printFlag[idCouple] == 0) {
        famLabelBegin = "<table border=\"0\" cellborder=\"0\" cellpadding=\"3\" bgcolor=\"white\">"

        if (idIndi != "" && tabIndi_printFlag[idIndi] == 0) {
            tabIndi_printFlag[idIndi] = 1
            indiShape = "Mrecord"
            if (tabIndi_sex[idIndi] == SEX_MALE) {
                bgcolor = MALE_COLOR_BG
                fgcolor = MALE_COLOR_FG
            } else if (tabIndi_sex[idIndi] == SEX_FEMALE) {
                bgcolor = FEMALE_COLOR_BG
                fgcolor = FEMALE_COLOR_FG
            } else {
                bgcolor = UNKNOWN_COLOR_BG
                fgcolor = UNKNOWN_COLOR_FG
            }
            indiLabelBegin = ""
            indiLabelId    = ""
            indiLabelOccu  = ""
            indiLabelBirt  = ""
            indiLabelChr   = ""
            indiLabelDeat  = ""
            indiLabelBuri  = ""
            indiLabelUls   = ""
            indilabelEnd   = ""
            indiLabel      = ""
            indiOccu       = transcoOccu(tabIndi_occu[idIndi])
            indiLabelBegin = "<table border=\"0\" cellborder=\"0\" cellpadding=\"3\" bgcolor=\"white\">"
            indiLabelId    = "<tr><td border=\"0\" bgcolor=\"" bgcolor "\" align=\"center\"><font color=\"" fgcolor "\">" transcoName(idIndi) "</font></td></tr>"
            if (indiOccu != "") {
                indiLabelOccu = "<tr><td align=\"left\">" indiOccu "</td></tr>"
            }
            if (tabIndi_stillBorn[idIndi] == 1) {
                if (tabIndi_birtDate[idIndi] != "" || tabIndi_birtPlace[idIndi] != "") {
                    indiLabelBirt = "<tr><td align=\"left\">" STILLBORN_DEATH " " transcoDate(tabIndi_birtDate[idIndi]) transcoPlace(tabIndi_birtPlace[idIndi]) "</td></tr>"
                }
            } else {
                if (tabIndi_birtDate[idIndi] != "" || tabIndi_birtPlace[idIndi] != "") {
                    indiLabelBirt = "<tr><td align=\"left\">" BIRTH_GLYPH " " transcoDate(tabIndi_birtDate[idIndi]) transcoPlace(tabIndi_birtPlace[idIndi]) "</td></tr>"
                }
                if (tabIndi_chrDate[idIndi] != "" || tabIndi_chrPlace[idIndi] != "") {
                    indiLabelChr = "<tr><td align=\"left\">" BAPTISM_GLYPH " " transcoDate(tabIndi_chrDate[idIndi]) transcoPlace(tabIndi_chrPlace[idIndi]) "</td></tr>"
                }
                if (tabIndi_deatDate[idIndi] != "" || tabIndi_deatPlace[idIndi] != "") {
                    indiLabelDeat = "<tr><td align=\"left\">" DEATH_GLYPH " " transcoDate(tabIndi_deatDate[idIndi]) transcoPlace(tabIndi_deatPlace[idIndi]) "</td></tr>"
                }
                if (tabIndi_buriDate[idIndi] != "" || tabIndi_buriPlace[idIndi] != "") {
                    indiLabelBuri = "<tr><td align=\"left\">" BURIAL_GLYPH " " transcoDate(tabIndi_buriDate[idIndi]) transcoPlace(tabIndi_buriPlace[idIndi]) "</td></tr>"
                }
            }
            if (tabIndi_descCut[idIndi] == 1) {
                indiLabelUls = "<tr><td align=\"center\">" CUT_GLYPH_DESC "</td></tr>"
            }
            indilabelEnd = "</table>"
            indiLabel = "<" indiLabelBegin indiLabelId indiLabelOccu indiLabelBirt indiLabelChr indiLabelDeat indiLabelBuri indiLabelUls indilabelEnd ">"
            addLineToDotOutput("  \"INDI" idIndi "\" [shape=" indiShape ",label=" indiLabel ",regular=true,style=filled,fillcolor=white];")
        }

        
        tabFam_printFlag[idFamOutput] = 1
        famShape   = "Mrecord"        
        bgcolor    = COUPLE_COLOR_BG
        fgcolor    = COUPLE_COLOR_FG
        labelUnion = "<i>union</i>"
        if (tabFam_marrUst[idCouple] == "MARRIED") {
            if (   tabFam_marrDate[idCouple]    != "" || tabFam_marrPlace[idCouple]    != "" \
                || tabFam_marrRelDate[idCouple] != "" || tabFam_marrRelPlace[idCouple] != "" \
                || tabFam_divDate[idCouple]     != "" || tabFam_divPlace[idCouple]     != "") {
                labelUnion = "Mariage"
                if (tabFam_divDate[idCouple] != "" || tabFam_divPlace[idCouple] != "") {
                    labelUnion = labelUnion " et divorce"
                }
            }
        } else if (tabFam_marrUst[idCouple] == "COHABITATION") {
            labelUnion = "Union libre"
            if (tabFam_divDate[idCouple] != "" || tabFam_divPlace[idCouple] != "") {
                labelUnion = labelUnion " et séparation"
            }
        }
        famLabelId    = "<tr><td bgcolor=\"" bgcolor "\" align=\"center\"><font color=\"" fgcolor "\">" labelUnion "</font></td></tr>"
        if (tabFam_marrDate[idCouple] != "" || tabFam_marrPlace[idCouple] != "") {
            famLabelMarr = "<tr><td align=\"left\">" COUPLE_GLYPH_UNION " " transcoDate(tabFam_marrDate[idCouple]) transcoPlace(tabFam_marrPlace[idCouple]) "</td></tr>"
        } else {
            famLabelMarr = ""
        }
        if (tabFam_marrRelDate[idCouple] != "" || tabFam_marrRelPlace[idCouple] != "") {
            famLabelMarrRel = "<tr><td align=\"left\">" COUPLE_GLYPH_UNION_RELIGIOUS " " transcoDate(tabFam_marrRelDate[idCouple]) transcoPlace(tabFam_marrRelPlace[idCouple]) "</td></tr>"
        } else {
            famLabelMarrRel = ""
        }
        if (tabFam_divDate[idCouple] != "" || tabFam_divPlace[idCouple] != "") {
            famLabelDiv = "<tr><td align=\"left\">" COUPLE_GLYPH_UNION_SEPARATION " " transcoDate(tabFam_divDate[idCouple]) transcoPlace(tabFam_divPlace[idCouple]) "</td></tr>"
        } else {
            famLabelDiv = ""
        }
        famlabelEnd = "</table>"
        famLabel = "<" famLabelBegin famLabelId famLabelMarr famLabelMarrRel famLabelDiv famlabelEnd ">"
        addLineToDotOutput("  \"FAM" idCouple "\" [shape=" famShape ",label=" famLabel ",regular=true,style=filled,fillcolor=white];")
    }
}

function printAscIndiToDotOutput(idAscOrg, depthOrg, filterMode) {
    listIndiToPrint = id
    depth = depthOrg
    if (depth != -1) {
        listIndiToPrint = listIndiToPrint " ,"
    }
    while (listIndiToPrint != "") {
        printStdErr("listIndiToPrint = '" listIndiToPrint "'")
        printStdErr("depth = '" depth "'")
        idAsc = getHead(listIndiToPrint)
        printStdErr("idAsc = '" idAsc "'(" tabIndi_name[idAsc] ")")
        listIndiToPrint = getTail(listIndiToPrint)
        if (idAsc == ",") {
            if (depth != -1) {
                depth = depth - 1
                if (depth < 1) {
                    break
                }
            }
            idAsc = listIndiToPrint
            gsub(/ .*$/,     "", idAsc)
            gsub(/^[^ ]* ?/, "", listIndiToPrint)
            if (listIndiToPrint != "") {
                listIndiToPrint = listIndiToPrint " ,"
            }
        }
        if (idAsc == idAscOrg || filterMode == "NO") {
            addIndiToDotOutput(idAsc)
        } else if (tabIndi_sex[idAsc] == SEX_MALE && filterMode == "AGNATIC") {
            addIndiToDotOutput(idAsc)
        }
        if (tabIndi_fam[idAsc] != "") {
            if (depth == -1 || depth > 1) {
                if (FILTER_FRATRIE == "YES") {
                    addFamAndChildsToDotOutput(tabIndi_fam[idAsc])
                } else {
                    addCoupleHusbandAndWifeToDotOutput(tabIndi_fam[idAsc])
                    addLinkFamIndiToDotOutput(tabIndi_fam[idAsc], idAsc)
                }
            }
        }
        if (tabIndi_father[idAsc] != "") {
            listIndiToPrint = listIndiToPrint " " tabIndi_father[idAsc]
        }
        if (tabIndi_mother[idAsc] != "") {
            if (filterMode != "AGNATIC") {
                listIndiToPrint = listIndiToPrint " " tabIndi_mother[idAsc]
            }
        }
    }
}

function printDescIndiToDotOutput() {
}

function addFamAndChildsToDotOutput(idFamOutput) {
    if (idFamOutput != "" && tabFam_printFlag[idFamOutput] == 0) {
        addCoupleHusbandAndWifeToDotOutput(idFamOutput)

        split(tabFam_idChils[idFamOutput],tabChis,"#")
        for (idChi in tabChis) {
            addIndiToDotOutput(tabChis[idChi])
            addLinkFamIndiToDotOutput(idFamOutput, tabChis[idChi])
        }
    }
}

# TODO : Graph Description
BEGIN {
    initTemplates()
    initMacros()
    initVars()
    checkParameters()
}

/^[ \t]/ {
    gsub(/^[ \t]+/, "");
}
/^0 / {
    elt0 = ""
    id = ""
}
/^1 / {
    elt1 = ""
}

/^0 @[^@]+@ INDI */ {
    id = $2
    tabIndi[id] = 1
    elt0 = "INDI"
    next
}
elt0 == "INDI" && /^1 NAME / {
    gsub(/^1 NAME /, "")
    tabIndi_name[id] = $0
    if (match(toupper($0), FILTER_INDI) != 0) {
        tabIndi_matchFilterIndi[id] = 1
        printStdErr("Match = '" $0 "'")
    }
    tabIndi_printFlag[id] = 0
    next
}

elt0 == "INDI" && /^1 SEX / {
    gsub(/^1 SEX /, "")
    if ($0 == "M") {
        tabIndi_sex[id] = SEX_MALE
    } else if ($0 == "F") {
        tabIndi_sex[id] = SEX_FEMALE
    } else {
        tabIndi_sex[id] = SEX_UNKNOWN
    }
    next
}

elt0 == "INDI" && /^1 OCCU / {
    gsub(/^1 OCCU /, "")
    tabIndi_occu[id] = $0
    next
}

#elt0 == "INDI" && /^1 _ULS YES$/ {
#    tabIndi_descCut[id] = 1
#    next
#}

elt0 == "INDI" && /^1 _FIL STILLBORN_CHILD$/ {
    tabIndi_descCut[id]   = 1
    tabIndi_stillBorn[id] = 1
    next
}

#elt0 == "INDI" && /^1 _CLS YES$/ {
#    tabIndi_descCut[id] = 1
#    next
#}

elt0 == "INDI" && /^1 BIRT *$/ {
    elt1 = "BIRT"
    next
}
elt0 == "INDI" && elt1 == "BIRT" && /^2 DATE / {
    gsub(/^2 DATE /, "")
    tabIndi_birtDate[id] = $0
    next
}
elt0 == "INDI" && elt1 == "BIRT" && /^2 PLAC / {
    gsub(/^2 PLAC /, "")
    tabIndi_birtPlace[id] = $0
    next
}
elt0 == "INDI" && /^1 CHR *$/ {
    elt1 = "CHR"
    next
}
elt0 == "INDI" && elt1 == "CHR" && /^2 DATE / {
    gsub(/^2 DATE /, "")
    tabIndi_chrDate[id] = $0
    next
}
elt0 == "INDI" && elt1 == "CHR" && /^2 PLAC / {
    gsub(/^2 PLAC /, "")
    tabIndi_chrPlace[id] = $0
    next
}

elt0 == "INDI" && /^1 DEAT *$/ {
    elt1 = "DEAT"
    next
}
elt0 == "INDI" && elt1 == "DEAT" && /^2 DATE / {
    gsub(/^2 DATE /, "")
    tabIndi_deatDate[id] = $0
    next
}
elt0 == "INDI" && elt1 == "DEAT" && /^2 PLAC / {
    gsub(/^2 PLAC /, "")
    tabIndi_deatPlace[id] = $0
    next
}
elt0 == "INDI" && /^1 BURI *$/ {
    elt1 = "BURI"
    next
}
elt0 == "INDI" && elt1 == "BURI" && /^2 DATE / {
    gsub(/^2 DATE /, "")
    tabIndi_buriDate[id] = $0
    next
}
elt0 == "INDI" && elt1 == "BURI" && /^2 PLAC / {
    gsub(/^2 PLAC /, "")
    tabIndi_buriPlace[id] = $0
    next
}

/^0 @[^@]+@ FAM */ {
    id = $2
    tabFam[id] = 1
    elt0 = "FAM"
    tabFam_printFlag[id] = 0
    next
}

elt0 == "FAM" && /^1 HUSB / {
    gsub(/^1 HUSB /, "")
    tabFam_idHusb[id] = $1
    next
}
elt0 == "FAM" && /^1 WIFE / {
    gsub(/^1 WIFE /, "")
    tabFam_idWife[id] = $1
    next
}
elt0 == "FAM" && /^1 CHIL / {
    gsub(/^1 CHIL /, "")
    if (tabFam_idChils[id] == "") {
        tabFam_idChils[id] = $1
    } else {
        tabFam_idChils[id] = tabFam_idChils[id] SEP $1
    }
    if (tabFam_idHusb[id] != "") {
        tabIndi_father[$1] = tabFam_idHusb[id]
        if (tabIndi_idChils[tabFam_idHusb[id]] == "") {
            tabIndi_idChils[tabFam_idHusb[id]] = $1
        } else {
            tabIndi_idChils[tabFam_idHusb[id]] = tabIndi_idChils[tabFam_idHusb[id]] SEP $1
        }
    }
    if (tabFam_idWife[id] != "") {
        tabIndi_mother[$1] = tabFam_idWife[id]
        if (tabIndi_idChils[tabFam_idWife[id]] == "") {
            tabIndi_idChils[tabFam_idWife[id]] = $1
        } else {
            tabIndi_idChils[tabFam_idWife[id]] = tabIndi_idChils[tabFam_idWife[id]] SEP $1
        }
    }
    if (tabFam_idHusb[id] != "" || tabFam_idWife[id] != "") {
        tabIndi_fam[$1] = id
    }
    next
}
elt0 == "FAM" && /^1 NCHI / {
    gsub(/^1 NCHI /, "")
    tabFam_nChi[id] = $1
    next
}
elt0 == "FAM" && /^1 _UST / {
    gsub(/^1 _UST /, "")
    tabFam_marrUst[id] = $1
    next
}
elt0 == "FAM" && /^1 MARR *$/ {
    elt1 = "MARR"
    next
}
elt0 == "FAM" && elt1 == "MARR" && /^2 TYPE Religious marriage$/ {
    tabFam_marrRel[id] = 1
    next
}
elt0 == "FAM" && elt1 == "MARR" && /^2 DATE / {
    gsub(/^2 DATE /, "")
    if (tabFam_marrRel[id] == 1) {
        tabFam_marrRelDate[id] = $0
    } else {
        tabFam_marrDate[id] = $0
    }
    next
}
elt0 == "FAM" && elt1 == "MARR" && /^2 PLAC / {
    gsub(/^2 PLAC /, "")
    if (tabFam_marrRel[id] == 1) {
        tabFam_marrRelPlace[id] = $0
    } else {
        tabFam_marrPlace[id] = $0
    }
    next
}
elt0 == "FAM" && /^1 DIV *$/ {
    elt1 = "DIV"
    next
}
elt0 == "FAM" && elt1 == "DIV" && /^2 DATE / {
    gsub(/^2 DATE /, "")
    tabFam_divDate[id] = $0
    next
}
elt0 == "FAM" && elt1 == "DIV" && /^2 PLAC / {
    gsub(/^2 PLAC /, "")
    tabFam_divPlace[id] = $0
    next
}

END {
    dotOutput   = ""
    addLineToDotOutput("digraph " GRAPH_NAME " {")
    addLineToDotOutput("  ratio = \"auto\";")
    addLineToDotOutput("  mincross = 2.0;")
    if (TITLE != "") {
        if (TITLE == "AUTO") {
            graphLabel = "Généalogie"
            if (FILTER_ORDER == "ASC") {
                graphLabel = graphLabel " ascendante"
            } else if (FILTER_ORDER == "ASC") {
                graphLabel = graphLabel " ascendante"
            }
            graphLabel = graphLabel " de '" FILTER_INDI "'"
            if (FILTER_DEPTH > 0) {
                graphLabel = graphLabel " sur " FILTER_DEPTH " génération(s)"
            }
            if (FILTER_FRATRIE == "YES") {
                graphLabel = graphLabel " avec fratries"
            }
        } else {
            graphLabel = TITLE
        }
        addLineToDotOutput("  label = \"" graphLabel "\";")
    }
    if (FILTER_ORDER == "NO") {
        for (idIndi in tabIndi) {
            addIndiToDotOutput(idIndi)
        }
        for (idFam in tabFam) {
            addFamAndChildsToDotOutput(idFam)
        }
    } else if (FILTER_ORDER == "ASC") {
        addLineToDotOutput("  rankdir = TB;")
        for (id in tabIndi) {
            if (tabIndi_matchFilterIndi[id] == 1) {
                printAscIndiToDotOutput(id, FILTER_DEPTH, FILTER_MODE)
            }
        }
    } else if (FILTER_ORDER == "DESC") {
        #addLineToDotOutput("  rankdir = BT;")
        for (id in tabIndi) {
            if (tabIndi_matchFilterIndi[id] == 1) {
                FILTER_DEPTH = FILTER_DEPTH_ORG
                listIndiToPrint = id
                if (FILTER_DEPTH != -1) {
                    listIndiToPrint = listIndiToPrint " ,"
                }
                printDescIndiToDotOutput()
            }
        }
    }
    addLineToDotOutput("}")
    print dotOutput
}
