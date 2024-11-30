#include "dico.h"


const char *dico_trois[] = {
    "aas", "ace", "ada", "ado", "aga", "age", "agi", "aid", "aie", "ail", "air", "ais", "ait", "ale", "alu", "ame", 
    "ami", "ana", "ane", "ani", "ans", "api", "app", "ara", "arc", "are", "arf", "ars", "art", "asa", "ase", "aux", 
    "ave", "axa", "axe", "ays", "bac", "bah", "bai", "bal", "ban", "bao", "bar", "bas", "bat", "bau", "bea", "bec", 
    "bee", "bel", "ben", "ber", "beu", "bey", "bib", "bic", "bim", "bio", "bip", "bis", "bit", "ble", "boa", "bob", 
    "bof", "bog", "bol", "bon", "bop", "bot", "box", "boy", "bru", "bue", "bug", "bun", "bus", "but", "bye", "cab", 
    "caf", "cal", "cap", "car", "cas", "cep", "ces", "cet", "chu", "cif", "cil", "cis", "cle", "cob", "coi", "col", 
    "com", "con", "coq", "cor", "cot", "cou", "cox", "cre", "cri", "cru", "cul", "cut", "dab", "dah", "dal", "dam", 
    "dan", "dao", "daw", "deb", "dej", "del", "dem", "deo", "der", "des", "dey", "dia", "din", "dip", "dis", "dit", 
    "dix", "doc", "dol", "dom", "don", "dop", "dos", "dot", "dru", "dry", "dub", "duc", "due", "duo", "dur", "dus", 
    "dut", "dys", "dzo", "eau", "eco", "ecu", "ego", "elu", "emu", "eon", "epi", "ere", "erg", "ers", "est", "eta", 
    "ete", "eue", "euh", "eus", "eut", "eux", "eve", "ewe", "exo", "fac", "faf", "fan", "faq", "far", "fat", "fax", 
    "fee", "fer", "feu", "fez", "fia", "fic", "fie", "fil", "fin", "fis", "fit", "fiu", "fix", "fla", "fob", "foc", 
    "fog", "foi", "fol", "fon", "for", "fou", "fox", "fui", "fun", "fur", "fus", "fut", "gag", "gai", "gal", "gan", 
    "gap", "gay", "gaz", "gel", "geo", "gex", "ghi", "gif", "gin", "gis", "git", "glu", "goi", "gon", "gos", "goy", 
    "gre", "gue", "gui", "gur", "gus", "gym", "hai", "han", "hem", "hep", "heu", "hia", "hic", "hie", "hip", "hit", 
    "hop", "hot", "hou", "hua", "hub", "hue", "hui", "hum", "hun", "ibn", "ibo", "ici", "ide", "iel", "ifs", "ile", 
    "ils", "ion", "ipe", "ira", "ire", "iso", "ive", "ixa", "ixe", "jab", "jam", "jan", "jar", "jas", "jet", "jeu", 
    "job", "jus", "kan", "kas", "kat", "kea", "ken", "ket", "khi", "kid", "kif", "kil", "kip", "kir", "kit", "kob", 
    "koi", "kop", "kot", "kru", "ksi", "kwa", "kyu", "lac", "lad", "lai", "lao", "las", "led", "lei", "lek", "lem", 
    "les", "let", "leu", "lev", "lez", "lia", "lie", "lin", "lis", "lit", "loa", "lob", "lof", "log", "loi", "lol", 
    "los", "lot", "lue", "lui", "luo", "lus", "lut", "lux", "lys", "mac", "mag", "mai", "mal", "man", "mao", "mas", 
    "mat", "max", "mec", "mel", "meo", "mer", "mes", "met", "mie", "mil", "min", "mir", "mis", "mit", "mix", "mmm", 
    "moa", "mob", "moi", "mol", "mon", "mor", "mos", "mot", "mou", "mox", "mua", "mue", "mug", "mur", "mus", "mut", 
    "mye", "nac", "nan", "nay", "nee", "nef", "nem", "neo", "nes", "net", "ney", "nez", "nia", "nib", "nid", "nie", 
    "nif", "nim", "nit", "nom", "non", "nos", "nua", "nue", "nui", "nul", "nus", "oba", "obi", "ode", "off", "ohe", 
    "ohm", "oie", "oil", "ois", "oit", "oka", "ola", "ole", "onc", "ont", "ope", "ore", "ors", "osa", "ose", "ost", 
    "ota", "ote", "oud", "ouf", "ouh", "oui", "out", "ove", "oxo", "oye", "paf", "pal", "pan", "pap", "par", "pas", 
    "pat", "pec", "pep", "pet", "peu", "pff", "phi", "pho", "pic", "pie", "pif", "pin", "pis", "piu", "pli", "plu", 
    "pop", "pot", "pou", "pre", "pro", "psi", "pst", "psy", "pua", "pub", "pue", "pur", "pus", "put", "puy", "qat", 
    "qin", "qis", "que", "qui", "rab", "rac", "rad", "rai", "ram", "rap", "ras", "rat", "ray", "raz", "rea", "ree", 
    "reg", "rem", "rez", "rhe", "rho", "ria", "rib", "rie", "rif", "rio", "ris", "rit", "riz", "rob", "roc", "roi", 
    "rom", "ros", "rot", "rua", "rue", "rus", "rut", "ruz", "rye", "sac", "sai", "sal", "sar", "sas", "sax", "sec", 
    "sel", "sen", "sep", "ses", "set", "sic", "sil", "sir", "sis", "six", "ska", "ski", "soc", "soi", "sol", "som", 
    "son", "sot", "sou", "spa", "spi", "sua", "suc", "sud", "sue", "sup", "sur", "sus", "sut", "tac", "taf", "tag", 
    "tan", "tao", "tar", "tas", "tat", "tau", "tec", "tee", "tef", "tek", "tel", "tep", "ter", "tes", "tet", "tex", 
    "the", "tic", "tif", "tin", "tip", "tir", "toc", "tof", "toi", "tom", "ton", "top", "tos", "tot", "tri", "tua", 
    "tub", "tue", "tuf", "tus", "tut", "uds", "une", "uni", "uns", "ure", "usa", "use", "ute", "val", "van", "var", 
    "vas", "vat", "vau", "ver", "ves", "vet", "via", "vie", "vif", "vil", "vin", "vis", "vit", "voc", "vol", "vos", 
    "vue", "vus", "wad", "wap", "wax", "web", "woh", "wok", "won", "wus", "xis", "yak", "yam", "yen", "yet", "yin", 
    "yod", "yue"
};

const size_t DICO_TROIS_SIZE = sizeof(dico_trois) / sizeof(dico_trois[0]);

