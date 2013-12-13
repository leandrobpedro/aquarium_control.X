/**
 * Projeto      : aquario
 * Arquivo      : main.c
 *
 * Alunos       : Thyago | Flavio
 * Data         : 30-oct-2013
 * Atualizado   : 28-nov-2013
 */

#include <16F877A.h>
#include <Other Files/flex_lcd.c>
#include <aquaproj.h>


#use delay(clock = 3M)
#fuses RC, NOWDT, NOBROWNOUT, NODEBUG, NOPUT, NOPROTECT

static long int ligthDelay = 5; // intervalo de iluminacao, definir aqui valor inicial
static int contLight = 0;       // variavel usada p/ contar ciclos

static long int feedDelay = 4;  // intervalo de alimentacao, definir aqui valor inicial
static int contFeed = 0;        // variavel usada p/ contar ciclos
static int contFeed1 = 0;       // variavel usada p/ contar ciclos
static int feedTime = 1;        // perido da alimentacao

char teclado () {
    // TODO: melhorar varredura

    /* ativa coluna 0 do teclado  */
    output_bit(col0, TRUE);         // ativa    col0
    output_bit(col1, FALSE);        // desativa col1
    output_bit(col2, FALSE);        // desativa col2
    delay_ms(5);
    if (input(lin0)) {return '1';}  // caso col0 e lin0 ativas, retorna '1'
    if (input(lin1)) {return '4';}  // caso col0 e lin1 ativas, retorna '4'
    if (input(lin2)) {return '7';}  // caso col0 e lin2 ativas, retorna '7'
    if (input(lin3)) {return '*';}  // caso col0 e lin3 ativas, retorna '*'
    //------------------------------

    /* ativa coluna 1 do teclado  */
    output_bit(col0, FALSE);        // desativa col0
    output_bit(col1, TRUE);         // ativa    col1
    output_bit(col2, FALSE);        // desativa col3
    delay_ms(5);
    if (input(lin0)) {return '2';}  // caso col1 e lin0 ativas, retorna '2'
    if (input(lin1)) {return '5';}  // caso col1 e lin1 ativas, retorna '5'
    if (input(lin2)) {return '8';}  // caso col1 e lin2 ativas, retorna '8'
    if (input(lin3)) {return '0';}  // caso col1 e lin3 ativas, retorna '0'
    //------------------------------
    
    /* ativa coluna 2 do teclado  */
    output_bit(col0, FALSE);        // desativa col0
    output_bit(col1, FALSE);        // desativa col1
    output_bit(col2, TRUE);         // ativa    col2
    delay_ms(5);
    if (input(lin0)) {return '3';}  // caso col2 e lin0 ativas, retorna '3'
    if (input(lin1)) {return '6';}  // caso col2 e lin1 ativas, retorna '6'
    if (input(lin2)) {return '9';}  // caso col2 e lin2 ativas, retorna '9'
    if (input(lin3)) {return '#';}  // caso col2 e lin3 ativas, retorna '#'
    //------------------------------

    return 0;                       // caso nehuma linha ativa retorna NULL

}

char tecladodb(unsigned long tempodb) {
    char tecla, tecla1;

    tecla = teclado();      // faz PRIMEIRA leitura e armazena tecla
    delay_ms(tempodb);      // aguarda tempo de "debouncing"
    tecla1 = teclado();     // faz SEGUNDA leitura e armazena tecla1

    if(tecla == tecla1)     // tecla do primeiro scan é igual tecla1 do segundo scan?
        return tecla;       // se sim, retorna tecla
    else
        return 0;			// retorna NULL
}

int getIntValue () {
    char tecla;
    int result, a, b;

    while((tecla = tecladodb(100)) == 0);
    a = tecla - '0';                        // transforma char em int
    lcd_putc(tecla);                        // imprime primeira tecla digitada no display lcd

    while((tecla = tecladodb(100)) == 0);
    if (tecla == '#') {                     // caso segunda tecla digitada '#'
        result = a;                         // se encerra captura do intervalo de ilumincao e retorna
                                            // somente o primeiro valor digitado
    }
    else {
        b = tecla - '0';                    // transforma char em int
        result = a * 10 + b;                // multiplica primeira tecla digitada x10 e soma a segunda
    }

    return result;                          // retorna result (tipo int)

}

void iniPrograma () {
    char frase_ini[] = "PROGRAMA\nINICIALIZANDO...";    // frase de boas vindas
    int i;

    lcd_init();                     // inicializa/limpa display lcd
    lcd_gotoxy(5,1);                // coloca cursor na posicao 5x1 p/ centralizar _ _ _ _ P R O G R A M A _ _ _ _
                                    //                                              I N I C I A L I Z A N D O . . .
    for (i = 0;i < 26; i++) {       // laco p/ imprimir letra por letra
        lcd_putc(frase_ini[i]);     // imprime char da posicao i
        delay_ms(50);               // delay de 50 ms p/ efeito visual
    }

    for (i = 0;i < 3;i++) {                 // laco p/ frase de boas vindas piscar ix
        lcd_init();                         // inicializa/limpa display lcd
        lcd_gotoxy(5,1);                    // coloca cursor na posicao 5x1 pra centralizar frase
        delay_ms(500);
        printf(lcd_putc,"%s", frase_ini);   // imprime frase inteira [%s]
        delay_ms(300);                      // delay 300 ms p/ efeito visual
    }

    lcd_init();                             // inicializa/limpa display lcd
    lcd_gotoxy(1,1);                        // coloca cursor na posicao 1, 1
}

void confIluminacao () {
    lcd_init();                                    	// inicializa/limpa display lcd
    lcd_gotoxy(1,1);                                // coloca cursor na posicao 1, 1
    lcd_putc("TEMPO ILUMINACAO\n");
    delay_ms(1000);                                 // aguarda 1s p/ evitar de pegar algum digito
                                                    // que possa ser enviado mesmo considerando o "debouncing"
    lcd_putc("DIGITE: ");                           // so entao se deve digitar o intervalo desejado

getvalue:
    ligthDelay = (long int)getIntValue();

    if (!(ligthDelay > 0 && (ligthDelay <= 24))) {  // se ligthDelay nao corresponder ao range 1-24 (h)
        lcd_putc("\fVALOR INCORRETO!\nDIGITE: ");   // imprime mensagem erro no display lcd
        goto getvalue;                              // e recomeca funcao de captura do valor do intervalo
    }
    else
        contLight = 0;                              // zera contLight
}

void confAlimentacao () {
    // TODO: configuracao alimentacao
}

void lightControl () {
    contLight++;                        // incrementa variavel contLight
    if (contLight == (int)ligthDelay) { // se contLight for igual intervalo desejado
        contLight = 0;                  // zera contLight p/ recomeço da proxima contagem
        output_toggle(LED1);            // "inverte" estado do LED
    }
}

void feedControl () {
    contFeed++;                         // incrementa variavel contFeed
    if (contFeed >= (int)feedDelay) {   // se intervalo sem alimentacao (feedDelay) for maior igual a contFeed
        output_high(LED3);              // se inicia alimentacao (acende LED)
        contFeed1++;                    // increnta contFeed1 p/ intervalo no qual se esta "despejando" alimento no aquario
    }
    if (contFeed1 > feedTime) {         // se contFeed1 maior que o intervalo de alimentacao definido
        contFeed = 0;                   // zera contFeed
        contFeed1 = 0;                  // zera contFeed1
        output_low(LED3);               // encerra alimentacao (apaga LED)
    }
}

#int_timer0
void trata_t0 () {
    static long int cont;           // variavel auxiliar

    set_timer0(156 + get_timer0()); // ajusta temporizado p/ "estourar"
    cont++;                         // incrementa cont

    if (cont == 234) {              // se cont igual a
        cont = 0;                   // zera variavel cont
        lightControl ();            // chama funcao controle de ilumincao
        feedControl ();             // chama funcao controle de alimentacao
    }

}

void main () {
    char  tecla, tecla1;
    float leitura;

//    setup_wdt(WD);
    

    // configuracao do timer0
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_32);     // prescaler em 32
    setup_timer_1(T1_DISABLED);                     // desabilita timer1
    setup_timer_2(T2_DISABLED, 0, 1);               // desabilita timer2
    enable_interrupts(GLOBAL | INT_TIMER0);         // habilita interrupcoes
    set_timer0(156);                                // inicializa timer0
    //-------------------------------
    // configuracao entrada analogica
    setup_adc_ports(RA0_ANALOG);                    // configura porta RA0/AN0 como conversor A/D
    setup_adc(ADC_CLOCK_INTERNAL);
    set_adc_channel(0);
    delay_us(10);
    //-------------------------------


    output_low(LED1);   // inicializa c/ iluminacao apagada
    output_low(LED3);   // inicializa c/ alimentacao desligada

    iniPrograma();      // chama funcao iniPrograma com mensagens de inicializacao

    /* loop inifinito */
    while (TRUE) {
        tecla = tecladodb(100);
        leitura = read_adc();               // faz leitura sensor temperatura
        delay_us(10);
        leitura = (leitura * 50) / 255;     // converte entrada 8-bit (0-255) em °C (0-50)

        if (tecla == '#')                   // se tecla '#' entra em modo de conf. de ilumincao
            confIluminacao();               // chama funcao configura iluminacao
        
//        if (tecla == '*')                   // se tecla '*' entra em modo de conf. de alimentacao
//            confAlimentacao();               // chama funcao configura alimentacao

        
        if (leitura < 18)                   // temperatura minima (18 °C)
            output_high(LED2);              // liga "resistencia" (LED2)
        if (leitura > 25)                   // temperatura maxima (25 °C)
            output_low(LED2);               // desliga "resistencia" (LED2)

        printf(lcd_putc,"\fTEMP.\t: %g%cC\n", leitura, (char)223);              // (char)223 imprime '°' no display LCD

        /* TODO: comentar uma das linhas abaixo */
//        printf(lcd_putc, "LIGHT\t: %02d (%02d)", (int)ligthDelay, contLight);   // imprime tempo de ilumincao c/ contador
        printf(lcd_putc, "LIGHT\t: %02d h", (int)ligthDelay);                   // imprime tempo de ilumincao s/ contador mais chr 'h'

        delay_ms(1000);

    }
}