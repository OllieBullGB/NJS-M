#include "SNRMLModel.h"

SNRMLModel& SNRMLModel::instance() {
    static SNRMLModel instance;
    return instance;
}

void SNRMLModel::add_vectors(double *v1, double *v2, int size, double *result)
{
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] + v2[i];
}

void SNRMLModel::mul_vector_number(double *v1, double num, int size, double *result)
{
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] * num;
}

std::array<double, 3> SNRMLModel::convert_to_input(double pSignal, double pNoise)
{
    const double epsilon = 1e-12;

    double log_p_signal = std::log10(pSignal + epsilon);
    double log_p_noise  = std::log10(pNoise + epsilon);
    double snr_db       = 10.0 * std::log10((pSignal / pNoise) + epsilon);

    return {snr_db, log_p_signal, log_p_noise};
}

void SNRMLModel::score(double * input, double * output) {
    double var0[2];
    double var1[2];
    double var2[2];
    double var3[2];
    double var4[2];
    double var5[2];
    double var6[2];
    double var7[2];
    double var8[2];
    double var9[2];
    double var10[2];
    double var11[2];
    double var12[2];
    double var13[2];
    double var14[2];
    double var15[2];
    double var16[2];
    double var17[2];
    double var18[2];
    double var19[2];
    double var20[2];
    double var21[2];
    double var22[2];
    double var23[2];
    double var24[2];
    double var25[2];
    double var26[2];
    double var27[2];
    double var28[2];
    double var29[2];
    double var30[2];
    double var31[2];
    double var32[2];
    double var33[2];
    double var34[2];
    double var35[2];
    double var36[2];
    double var37[2];
    double var38[2];
    double var39[2];
    double var40[2];
    double var41[2];
    double var42[2];
    double var43[2];
    double var44[2];
    double var45[2];
    double var46[2];
    double var47[2];
    double var48[2];
    double var49[2];
    double var50[2];
    double var51[2];
    double var52[2];
    double var53[2];
    double var54[2];
    double var55[2];
    double var56[2];
    double var57[2];
    double var58[2];
    double var59[2];
    double var60[2];
    double var61[2];
    double var62[2];
    double var63[2];
    double var64[2];
    double var65[2];
    double var66[2];
    double var67[2];
    double var68[2];
    double var69[2];
    double var70[2];
    double var71[2];
    double var72[2];
    double var73[2];
    double var74[2];
    double var75[2];
    double var76[2];
    double var77[2];
    double var78[2];
    double var79[2];
    double var80[2];
    double var81[2];
    double var82[2];
    double var83[2];
    double var84[2];
    double var85[2];
    double var86[2];
    double var87[2];
    double var88[2];
    double var89[2];
    double var90[2];
    double var91[2];
    double var92[2];
    double var93[2];
    double var94[2];
    double var95[2];
    double var96[2];
    double var97[2];
    double var98[2];
    double var99[2];
    double var100[2];
    if (input[0] <= 3.7731807231903076) {
        memcpy(var100, (double[]){0.09849394091912192, 0.9015060590808781}, 2 * sizeof(double));
    } else {
        memcpy(var100, (double[]){0.7809602534959014, 0.21903974650409863}, 2 * sizeof(double));
    }
    double var101[2];
    if (input[0] <= 3.8914473056793213) {
        memcpy(var101, (double[]){0.10418550947753295, 0.8958144905224671}, 2 * sizeof(double));
    } else {
        memcpy(var101, (double[]){0.7885802042115166, 0.21141979578848336}, 2 * sizeof(double));
    }
    add_vectors(var100, var101, 2, var99);
    double var102[2];
    if (input[0] <= 3.5373525619506836) {
        memcpy(var102, (double[]){0.09232940428154009, 0.9076705957184599}, 2 * sizeof(double));
    } else {
        memcpy(var102, (double[]){0.7760299726783347, 0.22397002732166527}, 2 * sizeof(double));
    }
    add_vectors(var99, var102, 2, var98);
    double var103[2];
    if (input[0] <= 3.8689268827438354) {
        memcpy(var103, (double[]){0.10167582837074363, 0.8983241716292564}, 2 * sizeof(double));
    } else {
        memcpy(var103, (double[]){0.7842897106512088, 0.21571028934879125}, 2 * sizeof(double));
    }
    add_vectors(var98, var103, 2, var97);
    double var104[2];
    if (input[0] <= 4.435283184051514) {
        memcpy(var104, (double[]){0.12273915885813903, 0.877260841141861}, 2 * sizeof(double));
    } else {
        memcpy(var104, (double[]){0.7999132634622335, 0.20008673653776654}, 2 * sizeof(double));
    }
    add_vectors(var97, var104, 2, var96);
    double var105[2];
    if (input[0] <= 3.7673709392547607) {
        memcpy(var105, (double[]){0.0999825476545987, 0.9000174523454013}, 2 * sizeof(double));
    } else {
        memcpy(var105, (double[]){0.7839392354899964, 0.21606076451000358}, 2 * sizeof(double));
    }
    add_vectors(var96, var105, 2, var95);
    double var106[2];
    if (input[0] <= 3.767290949821472) {
        memcpy(var106, (double[]){0.09914109954733624, 0.9008589004526637}, 2 * sizeof(double));
    } else {
        memcpy(var106, (double[]){0.7810996705674785, 0.21890032943252147}, 2 * sizeof(double));
    }
    add_vectors(var95, var106, 2, var94);
    double var107[2];
    if (input[0] <= 3.770020008087158) {
        memcpy(var107, (double[]){0.09777985875375551, 0.9022201412462445}, 2 * sizeof(double));
    } else {
        memcpy(var107, (double[]){0.7842296362266219, 0.2157703637733781}, 2 * sizeof(double));
    }
    add_vectors(var94, var107, 2, var93);
    double var108[2];
    if (input[0] <= 3.4362215995788574) {
        memcpy(var108, (double[]){0.08617456701010141, 0.9138254329898986}, 2 * sizeof(double));
    } else {
        memcpy(var108, (double[]){0.7735980494601185, 0.22640195053988157}, 2 * sizeof(double));
    }
    add_vectors(var93, var108, 2, var92);
    double var109[2];
    if (input[0] <= 3.9238487482070923) {
        memcpy(var109, (double[]){0.10769730804527379, 0.8923026919547262}, 2 * sizeof(double));
    } else {
        memcpy(var109, (double[]){0.7856934347155229, 0.2143065652844771}, 2 * sizeof(double));
    }
    add_vectors(var92, var109, 2, var91);
    double var110[2];
    if (input[0] <= 3.7555959224700928) {
        memcpy(var110, (double[]){0.09755908712568476, 0.9024409128743153}, 2 * sizeof(double));
    } else {
        memcpy(var110, (double[]){0.7818880714226668, 0.21811192857733322}, 2 * sizeof(double));
    }
    add_vectors(var91, var110, 2, var90);
    double var111[2];
    if (input[0] <= 3.8914473056793213) {
        memcpy(var111, (double[]){0.10350108152600448, 0.8964989184739955}, 2 * sizeof(double));
    } else {
        memcpy(var111, (double[]){0.7847976445098748, 0.21520235549012529}, 2 * sizeof(double));
    }
    add_vectors(var90, var111, 2, var89);
    double var112[2];
    if (input[0] <= 3.7673709392547607) {
        memcpy(var112, (double[]){0.09847509847509847, 0.9015249015249015}, 2 * sizeof(double));
    } else {
        memcpy(var112, (double[]){0.7818012362769151, 0.21819876372308494}, 2 * sizeof(double));
    }
    add_vectors(var89, var112, 2, var88);
    double var113[2];
    if (input[0] <= 3.7698402404785156) {
        memcpy(var113, (double[]){0.09909664356358538, 0.9009033564364146}, 2 * sizeof(double));
    } else {
        memcpy(var113, (double[]){0.7846155957134429, 0.21538440428655717}, 2 * sizeof(double));
    }
    add_vectors(var88, var113, 2, var87);
    double var114[2];
    if (input[0] <= 3.7559430599212646) {
        memcpy(var114, (double[]){0.09732165499483059, 0.9026783450051694}, 2 * sizeof(double));
    } else {
        memcpy(var114, (double[]){0.782611674431351, 0.21738832556864895}, 2 * sizeof(double));
    }
    add_vectors(var87, var114, 2, var86);
    double var115[2];
    if (input[0] <= 3.7706326246261597) {
        memcpy(var115, (double[]){0.09695706381983947, 0.9030429361801605}, 2 * sizeof(double));
    } else {
        memcpy(var115, (double[]){0.7813224342006008, 0.21867756579939926}, 2 * sizeof(double));
    }
    add_vectors(var86, var115, 2, var85);
    double var116[2];
    if (input[0] <= 3.7601559162139893) {
        memcpy(var116, (double[]){0.09893635638866743, 0.9010636436113326}, 2 * sizeof(double));
    } else {
        memcpy(var116, (double[]){0.782836189848385, 0.21716381015161504}, 2 * sizeof(double));
    }
    add_vectors(var85, var116, 2, var84);
    double var117[2];
    if (input[0] <= 3.7671146392822266) {
        memcpy(var117, (double[]){0.09765830299963016, 0.9023416970003698}, 2 * sizeof(double));
    } else {
        memcpy(var117, (double[]){0.7811445039110745, 0.2188554960889255}, 2 * sizeof(double));
    }
    add_vectors(var84, var117, 2, var83);
    double var118[2];
    if (input[0] <= 3.7691339254379272) {
        memcpy(var118, (double[]){0.09687045828892549, 0.9031295417110745}, 2 * sizeof(double));
    } else {
        memcpy(var118, (double[]){0.7782703268261791, 0.22172967317382086}, 2 * sizeof(double));
    }
    add_vectors(var83, var118, 2, var82);
    double var119[2];
    if (input[0] <= 4.41460394859314) {
        memcpy(var119, (double[]){0.1256398579105565, 0.8743601420894435}, 2 * sizeof(double));
    } else {
        memcpy(var119, (double[]){0.7988983099728904, 0.20110169002710965}, 2 * sizeof(double));
    }
    add_vectors(var82, var119, 2, var81);
    double var120[2];
    if (input[0] <= 3.7671146392822266) {
        memcpy(var120, (double[]){0.10023255813953488, 0.8997674418604651}, 2 * sizeof(double));
    } else {
        memcpy(var120, (double[]){0.7840948198725273, 0.2159051801274727}, 2 * sizeof(double));
    }
    add_vectors(var81, var120, 2, var80);
    double var121[2];
    if (input[0] <= 3.7714931964874268) {
        memcpy(var121, (double[]){0.09782481581101625, 0.9021751841889838}, 2 * sizeof(double));
    } else {
        memcpy(var121, (double[]){0.7832512990663175, 0.21674870093368248}, 2 * sizeof(double));
    }
    add_vectors(var80, var121, 2, var79);
    double var122[2];
    if (input[0] <= 3.7555359601974487) {
        memcpy(var122, (double[]){0.09846916858671458, 0.9015308314132854}, 2 * sizeof(double));
    } else {
        memcpy(var122, (double[]){0.7813471360692328, 0.21865286393076722}, 2 * sizeof(double));
    }
    add_vectors(var79, var122, 2, var78);
    double var123[2];
    if (input[0] <= 3.5901516675949097) {
        memcpy(var123, (double[]){0.0935969347448254, 0.9064030652551746}, 2 * sizeof(double));
    } else {
        memcpy(var123, (double[]){0.7811196696145957, 0.21888033038540436}, 2 * sizeof(double));
    }
    add_vectors(var78, var123, 2, var77);
    double var124[2];
    if (input[0] <= 3.847944498062134) {
        memcpy(var124, (double[]){0.09978575978074154, 0.9002142402192584}, 2 * sizeof(double));
    } else {
        memcpy(var124, (double[]){0.7841837861718577, 0.21581621382814226}, 2 * sizeof(double));
    }
    add_vectors(var77, var124, 2, var76);
    double var125[2];
    if (input[0] <= 3.7722203731536865) {
        memcpy(var125, (double[]){0.09827880954231098, 0.9017211904576891}, 2 * sizeof(double));
    } else {
        memcpy(var125, (double[]){0.7833770802699026, 0.21662291973009742}, 2 * sizeof(double));
    }
    add_vectors(var76, var125, 2, var75);
    double var126[2];
    if (input[0] <= 3.768195390701294) {
        memcpy(var126, (double[]){0.09876759194463883, 0.9012324080553612}, 2 * sizeof(double));
    } else {
        memcpy(var126, (double[]){0.7840080220882155, 0.2159919779117845}, 2 * sizeof(double));
    }
    add_vectors(var75, var126, 2, var74);
    double var127[2];
    if (input[0] <= 3.538893938064575) {
        memcpy(var127, (double[]){0.08971705997055195, 0.9102829400294481}, 2 * sizeof(double));
    } else {
        memcpy(var127, (double[]){0.774521862539704, 0.22547813746029602}, 2 * sizeof(double));
    }
    add_vectors(var74, var127, 2, var73);
    double var128[2];
    if (input[0] <= 3.7673709392547607) {
        memcpy(var128, (double[]){0.09942646058131623, 0.9005735394186838}, 2 * sizeof(double));
    } else {
        memcpy(var128, (double[]){0.78058729809911, 0.21941270190089002}, 2 * sizeof(double));
    }
    add_vectors(var73, var128, 2, var72);
    double var129[2];
    if (input[0] <= 3.767196297645569) {
        memcpy(var129, (double[]){0.09964953271028038, 0.9003504672897197}, 2 * sizeof(double));
    } else {
        memcpy(var129, (double[]){0.7821577048145658, 0.2178422951854342}, 2 * sizeof(double));
    }
    add_vectors(var72, var129, 2, var71);
    double var130[2];
    if (input[0] <= 3.765668034553528) {
        memcpy(var130, (double[]){0.09885820808575091, 0.9011417919142491}, 2 * sizeof(double));
    } else {
        memcpy(var130, (double[]){0.7812770637157193, 0.21872293628428072}, 2 * sizeof(double));
    }
    add_vectors(var71, var130, 2, var70);
    double var131[2];
    if (input[0] <= 3.4366191625595093) {
        memcpy(var131, (double[]){0.08707746265949085, 0.9129225373405092}, 2 * sizeof(double));
    } else {
        memcpy(var131, (double[]){0.7732474005788402, 0.22675259942115983}, 2 * sizeof(double));
    }
    add_vectors(var70, var131, 2, var69);
    double var132[2];
    if (input[0] <= 3.447178602218628) {
        memcpy(var132, (double[]){0.08687474989995998, 0.91312525010004}, 2 * sizeof(double));
    } else {
        memcpy(var132, (double[]){0.7768740826521956, 0.22312591734780443}, 2 * sizeof(double));
    }
    add_vectors(var69, var132, 2, var68);
    double var133[2];
    if (input[0] <= 3.9347362518310547) {
        memcpy(var133, (double[]){0.10592855090508572, 0.8940714490949143}, 2 * sizeof(double));
    } else {
        memcpy(var133, (double[]){0.7846553208029096, 0.21534467919709044}, 2 * sizeof(double));
    }
    add_vectors(var68, var133, 2, var67);
    double var134[2];
    if (input[0] <= 3.767213821411133) {
        memcpy(var134, (double[]){0.10130276789856194, 0.898697232101438}, 2 * sizeof(double));
    } else {
        memcpy(var134, (double[]){0.7830554291309805, 0.21694457086901955}, 2 * sizeof(double));
    }
    add_vectors(var67, var134, 2, var66);
    double var135[2];
    if (input[0] <= 4.368749141693115) {
        memcpy(var135, (double[]){0.12076907670739034, 0.8792309232926097}, 2 * sizeof(double));
    } else {
        memcpy(var135, (double[]){0.7985574148874783, 0.20144258511252164}, 2 * sizeof(double));
    }
    add_vectors(var66, var135, 2, var65);
    double var136[2];
    if (input[0] <= 3.768195390701294) {
        memcpy(var136, (double[]){0.09758799634603797, 0.902412003653962}, 2 * sizeof(double));
    } else {
        memcpy(var136, (double[]){0.7836438070117595, 0.21635619298824046}, 2 * sizeof(double));
    }
    add_vectors(var65, var136, 2, var64);
    double var137[2];
    if (input[0] <= 3.862205743789673) {
        memcpy(var137, (double[]){0.09917609326086539, 0.9008239067391346}, 2 * sizeof(double));
    } else {
        memcpy(var137, (double[]){0.7858647158256435, 0.21413528417435643}, 2 * sizeof(double));
    }
    add_vectors(var64, var137, 2, var63);
    double var138[2];
    if (input[0] <= 3.925368547439575) {
        memcpy(var138, (double[]){0.10738829979630299, 0.892611700203697}, 2 * sizeof(double));
    } else {
        memcpy(var138, (double[]){0.7864154837270269, 0.2135845162729732}, 2 * sizeof(double));
    }
    add_vectors(var63, var138, 2, var62);
    double var139[2];
    if (input[0] <= 3.7601486444473267) {
        memcpy(var139, (double[]){0.09803653681005543, 0.9019634631899446}, 2 * sizeof(double));
    } else {
        memcpy(var139, (double[]){0.7798430287506678, 0.22015697124933226}, 2 * sizeof(double));
    }
    add_vectors(var62, var139, 2, var61);
    double var140[2];
    if (input[0] <= 3.767196297645569) {
        memcpy(var140, (double[]){0.09856054851087867, 0.9014394514891213}, 2 * sizeof(double));
    } else {
        memcpy(var140, (double[]){0.7828925710523428, 0.2171074289476572}, 2 * sizeof(double));
    }
    add_vectors(var61, var140, 2, var60);
    double var141[2];
    if (input[0] <= 4.553712606430054) {
        memcpy(var141, (double[]){0.12796899445313611, 0.8720310055468639}, 2 * sizeof(double));
    } else {
        memcpy(var141, (double[]){0.805015074637841, 0.19498492536215897}, 2 * sizeof(double));
    }
    add_vectors(var60, var141, 2, var59);
    double var142[2];
    if (input[0] <= 3.7714931964874268) {
        memcpy(var142, (double[]){0.09874660253025948, 0.9012533974697405}, 2 * sizeof(double));
    } else {
        memcpy(var142, (double[]){0.7816338814259528, 0.21836611857404722}, 2 * sizeof(double));
    }
    add_vectors(var59, var142, 2, var58);
    double var143[2];
    if (input[0] <= 3.922123074531555) {
        memcpy(var143, (double[]){0.104039074275466, 0.895960925724534}, 2 * sizeof(double));
    } else {
        memcpy(var143, (double[]){0.7863288319643603, 0.2136711680356397}, 2 * sizeof(double));
    }
    add_vectors(var58, var143, 2, var57);
    double var144[2];
    if (input[0] <= 3.7671912908554077) {
        memcpy(var144, (double[]){0.09862965702495426, 0.9013703429750457}, 2 * sizeof(double));
    } else {
        memcpy(var144, (double[]){0.7851486914874638, 0.2148513085125362}, 2 * sizeof(double));
    }
    add_vectors(var57, var144, 2, var56);
    double var145[2];
    if (input[0] <= 3.924139142036438) {
        memcpy(var145, (double[]){0.10580419181054997, 0.89419580818945}, 2 * sizeof(double));
    } else {
        memcpy(var145, (double[]){0.7888637884872824, 0.21113621151271753}, 2 * sizeof(double));
    }
    add_vectors(var56, var145, 2, var55);
    double var146[2];
    if (input[0] <= 3.767196297645569) {
        memcpy(var146, (double[]){0.1009007955077211, 0.8990992044922789}, 2 * sizeof(double));
    } else {
        memcpy(var146, (double[]){0.7830169282434377, 0.21698307175656226}, 2 * sizeof(double));
    }
    add_vectors(var55, var146, 2, var54);
    double var147[2];
    if (input[0] <= 3.455846667289734) {
        memcpy(var147, (double[]){0.08783088012599188, 0.9121691198740082}, 2 * sizeof(double));
    } else {
        memcpy(var147, (double[]){0.7780662776379892, 0.2219337223620108}, 2 * sizeof(double));
    }
    add_vectors(var54, var147, 2, var53);
    double var148[2];
    if (input[0] <= 4.111333131790161) {
        memcpy(var148, (double[]){0.11116352794655998, 0.88883647205344}, 2 * sizeof(double));
    } else {
        memcpy(var148, (double[]){0.7902426700725624, 0.2097573299274376}, 2 * sizeof(double));
    }
    add_vectors(var53, var148, 2, var52);
    double var149[2];
    if (input[0] <= 3.7584104537963867) {
        memcpy(var149, (double[]){0.09823613971016745, 0.9017638602898326}, 2 * sizeof(double));
    } else {
        memcpy(var149, (double[]){0.7817769532055246, 0.21822304679447538}, 2 * sizeof(double));
    }
    add_vectors(var52, var149, 2, var51);
    double var150[2];
    if (input[0] <= 3.904953718185425) {
        memcpy(var150, (double[]){0.10313918522916626, 0.8968608147708337}, 2 * sizeof(double));
    } else {
        memcpy(var150, (double[]){0.7870252110173256, 0.21297478898267436}, 2 * sizeof(double));
    }
    add_vectors(var51, var150, 2, var50);
    double var151[2];
    if (input[0] <= 3.822684168815613) {
        memcpy(var151, (double[]){0.10023242859063755, 0.8997675714093625}, 2 * sizeof(double));
    } else {
        memcpy(var151, (double[]){0.7848553696109941, 0.2151446303890059}, 2 * sizeof(double));
    }
    add_vectors(var50, var151, 2, var49);
    double var152[2];
    if (input[0] <= 3.7731807231903076) {
        memcpy(var152, (double[]){0.09884604888698297, 0.901153951113017}, 2 * sizeof(double));
    } else {
        memcpy(var152, (double[]){0.7820704404930012, 0.2179295595069988}, 2 * sizeof(double));
    }
    add_vectors(var49, var152, 2, var48);
    double var153[2];
    if (input[0] <= 3.770020008087158) {
        memcpy(var153, (double[]){0.09775108363947363, 0.9022489163605264}, 2 * sizeof(double));
    } else {
        memcpy(var153, (double[]){0.7824468377383372, 0.21755316226166274}, 2 * sizeof(double));
    }
    add_vectors(var48, var153, 2, var47);
    double var154[2];
    if (input[0] <= 3.903140664100647) {
        memcpy(var154, (double[]){0.10214651550900616, 0.8978534844909939}, 2 * sizeof(double));
    } else {
        memcpy(var154, (double[]){0.7859440869758154, 0.2140559130241846}, 2 * sizeof(double));
    }
    add_vectors(var47, var154, 2, var46);
    double var155[2];
    if (input[0] <= 3.4876538515090942) {
        memcpy(var155, (double[]){0.0882797882081293, 0.9117202117918707}, 2 * sizeof(double));
    } else {
        memcpy(var155, (double[]){0.77600162151206, 0.22399837848794}, 2 * sizeof(double));
    }
    add_vectors(var46, var155, 2, var45);
    double var156[2];
    if (input[0] <= 3.7675299644470215) {
        memcpy(var156, (double[]){0.09959801740623658, 0.9004019825937634}, 2 * sizeof(double));
    } else {
        memcpy(var156, (double[]){0.7799087633738373, 0.22009123662616273}, 2 * sizeof(double));
    }
    add_vectors(var45, var156, 2, var44);
    double var157[2];
    if (input[0] <= 3.7714931964874268) {
        memcpy(var157, (double[]){0.09952199785386792, 0.900478002146132}, 2 * sizeof(double));
    } else {
        memcpy(var157, (double[]){0.781566832904039, 0.21843316709596097}, 2 * sizeof(double));
    }
    add_vectors(var44, var157, 2, var43);
    double var158[2];
    if (input[0] <= 3.7673709392547607) {
        memcpy(var158, (double[]){0.09579162212071143, 0.9042083778792885}, 2 * sizeof(double));
    } else {
        memcpy(var158, (double[]){0.7807906810626666, 0.21920931893733345}, 2 * sizeof(double));
    }
    add_vectors(var43, var158, 2, var42);
    double var159[2];
    if (input[0] <= 3.7555359601974487) {
        memcpy(var159, (double[]){0.10058929925899994, 0.899410700741}, 2 * sizeof(double));
    } else {
        memcpy(var159, (double[]){0.7827012330760992, 0.2172987669239008}, 2 * sizeof(double));
    }
    add_vectors(var42, var159, 2, var41);
    double var160[2];
    if (input[0] <= 3.7601486444473267) {
        memcpy(var160, (double[]){0.09982077456557313, 0.9001792254344269}, 2 * sizeof(double));
    } else {
        memcpy(var160, (double[]){0.7842026580351388, 0.21579734196486128}, 2 * sizeof(double));
    }
    add_vectors(var41, var160, 2, var40);
    double var161[2];
    if (input[0] <= 3.641639828681946) {
        memcpy(var161, (double[]){0.09379438219977908, 0.9062056178002209}, 2 * sizeof(double));
    } else {
        memcpy(var161, (double[]){0.778182658708037, 0.22181734129196296}, 2 * sizeof(double));
    }
    add_vectors(var40, var161, 2, var39);
    double var162[2];
    if (input[0] <= 3.888279914855957) {
        memcpy(var162, (double[]){0.10116738988518106, 0.898832610114819}, 2 * sizeof(double));
    } else {
        memcpy(var162, (double[]){0.7868793392552764, 0.2131206607447236}, 2 * sizeof(double));
    }
    add_vectors(var39, var162, 2, var38);
    double var163[2];
    if (input[0] <= 3.7673709392547607) {
        memcpy(var163, (double[]){0.09829742470877001, 0.90170257529123}, 2 * sizeof(double));
    } else {
        memcpy(var163, (double[]){0.7816044658409799, 0.21839553415902016}, 2 * sizeof(double));
    }
    add_vectors(var38, var163, 2, var37);
    double var164[2];
    if (input[0] <= 3.770595669746399) {
        memcpy(var164, (double[]){0.09664109716410972, 0.9033589028358903}, 2 * sizeof(double));
    } else {
        memcpy(var164, (double[]){0.7829080543659372, 0.2170919456340627}, 2 * sizeof(double));
    }
    add_vectors(var37, var164, 2, var36);
    double var165[2];
    if (input[0] <= 3.8754831552505493) {
        memcpy(var165, (double[]){0.10490843619418073, 0.8950915638058192}, 2 * sizeof(double));
    } else {
        memcpy(var165, (double[]){0.7846138953424051, 0.21538610465759486}, 2 * sizeof(double));
    }
    add_vectors(var36, var165, 2, var35);
    double var166[2];
    if (input[0] <= 3.7680293321609497) {
        memcpy(var166, (double[]){0.09808663351970438, 0.9019133664802956}, 2 * sizeof(double));
    } else {
        memcpy(var166, (double[]){0.7802464371364777, 0.21975356286352235}, 2 * sizeof(double));
    }
    add_vectors(var35, var166, 2, var34);
    double var167[2];
    if (input[0] <= 3.7603049278259277) {
        memcpy(var167, (double[]){0.10051381483276782, 0.8994861851672322}, 2 * sizeof(double));
    } else {
        memcpy(var167, (double[]){0.7827654538448836, 0.21723454615511642}, 2 * sizeof(double));
    }
    add_vectors(var34, var167, 2, var33);
    double var168[2];
    if (input[0] <= 3.770595669746399) {
        memcpy(var168, (double[]){0.09725695529853079, 0.9027430447014692}, 2 * sizeof(double));
    } else {
        memcpy(var168, (double[]){0.7844755608480817, 0.21552443915191832}, 2 * sizeof(double));
    }
    add_vectors(var33, var168, 2, var32);
    double var169[2];
    if (input[0] <= 3.758234977722168) {
        memcpy(var169, (double[]){0.09684711017576679, 0.9031528898242333}, 2 * sizeof(double));
    } else {
        memcpy(var169, (double[]){0.7828728145354816, 0.21712718546451834}, 2 * sizeof(double));
    }
    add_vectors(var32, var169, 2, var31);
    double var170[2];
    if (input[0] <= 4.224821090698242) {
        memcpy(var170, (double[]){0.11553784860557768, 0.8844621513944223}, 2 * sizeof(double));
    } else {
        memcpy(var170, (double[]){0.7940180426306952, 0.20598195736930477}, 2 * sizeof(double));
    }
    add_vectors(var31, var170, 2, var30);
    double var171[2];
    if (input[0] <= 3.7671146392822266) {
        memcpy(var171, (double[]){0.09851775384195867, 0.9014822461580413}, 2 * sizeof(double));
    } else {
        memcpy(var171, (double[]){0.7816795149652959, 0.21832048503470414}, 2 * sizeof(double));
    }
    add_vectors(var30, var171, 2, var29);
    double var172[2];
    if (input[0] <= 3.8499326705932617) {
        memcpy(var172, (double[]){0.10304368029739777, 0.8969563197026023}, 2 * sizeof(double));
    } else {
        memcpy(var172, (double[]){0.7836627866932985, 0.21633721330670155}, 2 * sizeof(double));
    }
    add_vectors(var29, var172, 2, var28);
    double var173[2];
    if (input[0] <= 3.6416527032852173) {
        memcpy(var173, (double[]){0.09436276934644532, 0.9056372306535547}, 2 * sizeof(double));
    } else {
        memcpy(var173, (double[]){0.7810837946380115, 0.2189162053619885}, 2 * sizeof(double));
    }
    add_vectors(var28, var173, 2, var27);
    double var174[2];
    if (input[0] <= 3.8917816877365112) {
        memcpy(var174, (double[]){0.10231193926846101, 0.897688060731539}, 2 * sizeof(double));
    } else {
        memcpy(var174, (double[]){0.789730712135296, 0.210269287864704}, 2 * sizeof(double));
    }
    add_vectors(var27, var174, 2, var26);
    double var175[2];
    if (input[0] <= 3.758234977722168) {
        memcpy(var175, (double[]){0.09727104982033602, 0.9027289501796639}, 2 * sizeof(double));
    } else {
        memcpy(var175, (double[]){0.7800791665521317, 0.21992083344786828}, 2 * sizeof(double));
    }
    add_vectors(var26, var175, 2, var25);
    double var176[2];
    if (input[0] <= 3.8665435314178467) {
        memcpy(var176, (double[]){0.10212117117986529, 0.8978788288201347}, 2 * sizeof(double));
    } else {
        memcpy(var176, (double[]){0.7873729843163243, 0.21262701568367573}, 2 * sizeof(double));
    }
    add_vectors(var25, var176, 2, var24);
    double var177[2];
    if (input[0] <= 3.885111093521118) {
        memcpy(var177, (double[]){0.10173039620503563, 0.8982696037949643}, 2 * sizeof(double));
    } else {
        memcpy(var177, (double[]){0.7845207415412753, 0.21547925845872476}, 2 * sizeof(double));
    }
    add_vectors(var24, var177, 2, var23);
    double var178[2];
    if (input[0] <= 3.770595669746399) {
        memcpy(var178, (double[]){0.09982630418236109, 0.900173695817639}, 2 * sizeof(double));
    } else {
        memcpy(var178, (double[]){0.781477727247822, 0.21852227275217798}, 2 * sizeof(double));
    }
    add_vectors(var23, var178, 2, var22);
    double var179[2];
    if (input[0] <= 3.760095953941345) {
        memcpy(var179, (double[]){0.09928150259674333, 0.9007184974032567}, 2 * sizeof(double));
    } else {
        memcpy(var179, (double[]){0.7801027045532352, 0.2198972954467648}, 2 * sizeof(double));
    }
    add_vectors(var22, var179, 2, var21);
    double var180[2];
    if (input[0] <= 3.767297863960266) {
        memcpy(var180, (double[]){0.0987539484138519, 0.9012460515861481}, 2 * sizeof(double));
    } else {
        memcpy(var180, (double[]){0.780534140969163, 0.219465859030837}, 2 * sizeof(double));
    }
    add_vectors(var21, var180, 2, var20);
    double var181[2];
    if (input[0] <= 3.769869089126587) {
        memcpy(var181, (double[]){0.09866185010390568, 0.9013381498960943}, 2 * sizeof(double));
    } else {
        memcpy(var181, (double[]){0.7811391813508536, 0.21886081864914644}, 2 * sizeof(double));
    }
    add_vectors(var20, var181, 2, var19);
    double var182[2];
    if (input[0] <= 4.580167293548584) {
        memcpy(var182, (double[]){0.12968854951037578, 0.8703114504896242}, 2 * sizeof(double));
    } else {
        memcpy(var182, (double[]){0.8030708300698991, 0.19692916993010093}, 2 * sizeof(double));
    }
    add_vectors(var19, var182, 2, var18);
    double var183[2];
    if (input[0] <= 3.7601486444473267) {
        memcpy(var183, (double[]){0.09745481516180661, 0.9025451848381933}, 2 * sizeof(double));
    } else {
        memcpy(var183, (double[]){0.783314385897629, 0.21668561410237094}, 2 * sizeof(double));
    }
    add_vectors(var18, var183, 2, var17);
    double var184[2];
    if (input[0] <= 3.7840086221694946) {
        memcpy(var184, (double[]){0.10017502917152858, 0.8998249708284715}, 2 * sizeof(double));
    } else {
        memcpy(var184, (double[]){0.7819370253903299, 0.21806297460967003}, 2 * sizeof(double));
    }
    add_vectors(var17, var184, 2, var16);
    double var185[2];
    if (input[0] <= 3.767196297645569) {
        memcpy(var185, (double[]){0.09980308436177887, 0.9001969156382211}, 2 * sizeof(double));
    } else {
        memcpy(var185, (double[]){0.784104616734291, 0.21589538326570895}, 2 * sizeof(double));
    }
    add_vectors(var16, var185, 2, var15);
    double var186[2];
    if (input[0] <= 3.8894482851028442) {
        memcpy(var186, (double[]){0.10340081436026839, 0.8965991856397316}, 2 * sizeof(double));
    } else {
        memcpy(var186, (double[]){0.7863676805872213, 0.21363231941277874}, 2 * sizeof(double));
    }
    add_vectors(var15, var186, 2, var14);
    double var187[2];
    if (input[0] <= 4.010699272155762) {
        memcpy(var187, (double[]){0.10723592280675252, 0.8927640771932475}, 2 * sizeof(double));
    } else {
        memcpy(var187, (double[]){0.7892824577400576, 0.2107175422599425}, 2 * sizeof(double));
    }
    add_vectors(var14, var187, 2, var13);
    double var188[2];
    if (input[0] <= 3.760103225708008) {
        memcpy(var188, (double[]){0.09768542049497596, 0.902314579505024}, 2 * sizeof(double));
    } else {
        memcpy(var188, (double[]){0.7837978355156042, 0.21620216448439575}, 2 * sizeof(double));
    }
    add_vectors(var13, var188, 2, var12);
    double var189[2];
    if (input[0] <= 3.7601486444473267) {
        memcpy(var189, (double[]){0.09998253071682292, 0.900017469283177}, 2 * sizeof(double));
    } else {
        memcpy(var189, (double[]){0.783441504867719, 0.21655849513228095}, 2 * sizeof(double));
    }
    add_vectors(var12, var189, 2, var11);
    double var190[2];
    if (input[0] <= 3.769869089126587) {
        memcpy(var190, (double[]){0.09913995308835027, 0.9008600469116498}, 2 * sizeof(double));
    } else {
        memcpy(var190, (double[]){0.7807424435231175, 0.21925755647688244}, 2 * sizeof(double));
    }
    add_vectors(var11, var190, 2, var10);
    double var191[2];
    if (input[0] <= 3.7601486444473267) {
        memcpy(var191, (double[]){0.09756145180499425, 0.9024385481950058}, 2 * sizeof(double));
    } else {
        memcpy(var191, (double[]){0.7821812007011394, 0.21781879929886064}, 2 * sizeof(double));
    }
    add_vectors(var10, var191, 2, var9);
    double var192[2];
    if (input[0] <= 3.442139148712158) {
        memcpy(var192, (double[]){0.08606879854736205, 0.913931201452638}, 2 * sizeof(double));
    } else {
        memcpy(var192, (double[]){0.7740164439326174, 0.22598355606738263}, 2 * sizeof(double));
    }
    add_vectors(var9, var192, 2, var8);
    double var193[2];
    if (input[0] <= 3.7691339254379272) {
        memcpy(var193, (double[]){0.10032104290300613, 0.8996789570969939}, 2 * sizeof(double));
    } else {
        memcpy(var193, (double[]){0.7855260267955194, 0.21447397320448056}, 2 * sizeof(double));
    }
    add_vectors(var8, var193, 2, var7);
    double var194[2];
    if (input[0] <= 3.76712429523468) {
        memcpy(var194, (double[]){0.09816200705727247, 0.9018379929427275}, 2 * sizeof(double));
    } else {
        memcpy(var194, (double[]){0.7822060138994014, 0.21779398610059864}, 2 * sizeof(double));
    }
    add_vectors(var7, var194, 2, var6);
    double var195[2];
    if (input[0] <= 3.7671146392822266) {
        memcpy(var195, (double[]){0.09892519012882198, 0.901074809871178}, 2 * sizeof(double));
    } else {
        memcpy(var195, (double[]){0.7849213881896587, 0.21507861181034127}, 2 * sizeof(double));
    }
    add_vectors(var6, var195, 2, var5);
    double var196[2];
    if (input[0] <= 3.770595669746399) {
        memcpy(var196, (double[]){0.09946288872455533, 0.9005371112754447}, 2 * sizeof(double));
    } else {
        memcpy(var196, (double[]){0.7803368241898514, 0.21966317581014866}, 2 * sizeof(double));
    }
    add_vectors(var5, var196, 2, var4);
    double var197[2];
    if (input[0] <= 3.615959405899048) {
        memcpy(var197, (double[]){0.09228872048849375, 0.9077112795115062}, 2 * sizeof(double));
    } else {
        memcpy(var197, (double[]){0.7777897953113593, 0.22221020468864072}, 2 * sizeof(double));
    }
    add_vectors(var4, var197, 2, var3);
    double var198[2];
    if (input[0] <= 3.767196297645569) {
        memcpy(var198, (double[]){0.09876567321589, 0.90123432678411}, 2 * sizeof(double));
    } else {
        memcpy(var198, (double[]){0.7849974671759696, 0.21500253282403034}, 2 * sizeof(double));
    }
    add_vectors(var3, var198, 2, var2);
    double var199[2];
    if (input[0] <= 3.767196297645569) {
        memcpy(var199, (double[]){0.0984335680705382, 0.9015664319294618}, 2 * sizeof(double));
    } else {
        memcpy(var199, (double[]){0.7818169361468895, 0.21818306385311045}, 2 * sizeof(double));
    }
    add_vectors(var2, var199, 2, var1);
    mul_vector_number(var1, 0.01, 2, var0);
    memcpy(output, var0, 2 * sizeof(double));
}
