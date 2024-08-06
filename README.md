# Projeto Integrador II - Manufatura Avançada
*Equipe: Deivid Rodrigues, Luiz Greboge, Nina Cunha*

<p>Manufaturar uma mini esteira automatizada que permite a separação de peças com cores específicas. Este projeto inclui um microcontrolador que visar identificar as cores de peças por meio de um sensor TCS230 e realizar a separação das peças por meio de um circuito eletro-pneumático auxiliar.</p>

## Mecânica
<p>A parte mecânica consistiu na idealização de como seria a carcaça da esteira e o melhor jeito de integrar a parte elétrica, facilitando o manuseio após a montagem. Fizemos a modelagem em 3D no SolidWorks,
que envolveu várias medições dos componentes, e acarretou a definição das dimensões de cada parte da esteira. Após as modelagens, projetamos os encaixes para cortar o MDF na máquina a laser.
Ocorreram algumas situações adversas, a mais significativa foi que a pólia feita na impressora 3d não aguentaria a tensão exercida pela correia, então como solução, colocamos um eixo de alumínio.</p>

![Esteira](https://github.com/user-attachments/assets/bac5ffe8-2ecd-4682-b54d-166fd6e98d95)

## Eletrônica
<p>Com o suporte do Arduíno, usamos um sensor infravermelho para identificar quando a peça está na área de análise do sensor de cor, e os módulos relé para acionar os atuadores pneumáticos. O acionamento do motor para a movimentação da esteira foi feito com a ponte H, e o controle da sua velocidade com um potenciômetro. Mostramos o resultado da análise de cor no display LCD, que também auxilia na fase de calibração do sensor de cor.</p>

![Diagrama elétrico Equipe 6](https://github.com/user-attachments/assets/f74de820-a611-4388-b1f4-476564c1bfc8)

## Programação
<p>Na parte da programação, contamos com a biblioteca do Liquidcrystal I2C para utilizar o display LCD com mais facilidade. O código foi separado de maneira modular a fim de facilitar a manutenção do mesmo. A maior dificuldade do código foram os momentos onde o Arduíno travava por executar as funções rápido de mais no seu loop, para consertar isso fizemos uso de delays de 1ms, assim evitando erros e travas no processo. Algumas modificações foram feitas de última hora após a troca da ponte h pelo mosfet.</p>

![Fluxograma_E6](https://github.com/user-attachments/assets/fa37e42e-d832-4226-9dfd-4bf14f71ebf1)
