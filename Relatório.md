**Funcionalidades Implementadas**

1. **Modo Noturno**
No modo noturno, identificado quando o valor analógico do sensor LDR é inferior ao limite definido (600), o LED amarelo pisca a cada segundo, enquanto os LEDs vermelho e verde permanecem apagados.

2. **Modo Convencional**
No modo convencional, identificado quando o valor analógico do sensor LDR é superior ao limite definido, o sistema alterna entre os seguintes estados:
- LED verde aceso por 3 segundos.
- LED amarelo aceso por 2 segundos.
- LED vermelho aceso por 5 segundos.

3. **Abertura do Semáforo com Botão**
Quando o semáforo está no estado fechado (LED vermelho aceso) e o botão é pressionado, o LED vermelho apaga e o LED verde acende após 1 segundo.

4 **Alerta HTTP**
Se o botão for pressionado três vezes consecutivas no estado fechado, o sistema realiza uma requisição HTTP para o endpoint configurado ("http://www.google.com.br/") como forma de teste.
