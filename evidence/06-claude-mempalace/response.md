Que texto de abertura interessante — você escreveu em latim clássico descrevendo o projeto com precisão surpreendente. Aqui vai um resumo completo:

---

## MemPalace

O projeto foi criado pela atriz Milla Jovovich e pelo CEO de cripto Ben Sigman, e foi lançado em 6 de abril de 2026. Em apenas dois dias ganhou mais de 23.000 estrelas no GitHub e quase 3.000 forks.

### O que é

MemPalace é um sistema de memória de IA gratuito e open-source que dá a grandes modelos de linguagem memória persistente entre sessões. Ele armazena todos os dados de conversa de forma verbatim — ao invés de usar IA para extrair resumos — e usa busca vetorial para recuperar memórias relevantes. Roda inteiramente de forma local usando ChromaDB e SQLite, sem custo de API.

### A arquitetura inspirada no método dos loci

A arquitetura é inspirada na técnica mnemônica antiga do "palácio da memória" — aquela onde você imagina caminhar por um edifício e colocar memórias em salas específicas. No MemPalace isso se traduz em: *wings* (projetos/pessoas), *halls* (tipos de memória), *rooms* (tópicos específicos), *closets* (resumos comprimidos) e *drawers* (originais verbatim).

Restringir o escopo da busca progressivamente (wing → hall → room) aumenta a precisão de recuperação de 60,9% para 73,1%, e adicionar contexto de hall+room eleva isso para 94,8% — uma melhoria de 34% sobre busca plana.

### O dialeto AAAK

AAAK é um dialeto de abreviação com perdas para empacotar entidades repetidas em menos tokens em escala. Legível por qualquer LLM que leia texto — Claude, GPT, Gemini, Llama, Mistral — sem necessidade de decodificador especial. O AAAK comprime passagens aproximadamente 30x (um trecho de 1.000 tokens para ~120 tokens).

### Benchmarks e controvérsia

O projeto reivindica o maior escore no benchmark LongMemEval (96,6% raw, 100% hybrid) entre ferramentas gratuitas. Porém esses números foram questionados por revisores independentes — o escore de 100% foi obtido após correções direcionadas para questões específicas que falhavam.

A comunidade de desenvolvedores também questionou o real envolvimento técnico de Jovovich, alegando que ela seria mais o rosto do projeto do que a programadora.

### Pontos fortes reais

Como o MemPalace roda inteiramente de forma local — sem dependência de nuvem e sem chave de API — seus dados nunca saem da sua infraestrutura. Para projetos sujeitos ao GDPR, isso elimina a necessidade de acordos de processamento de dados com fornecedores de memória.

A diferença de custo é de 250x: com MemPalace, o contexto de memória é pré-computado localmente — as 15 memórias mais importantes já estão selecionadas e truncadas, custando apenas ~800 tokens como entrada para o LLM. Com a abordagem ingênua, você enviaria todas as memórias armazenadas ao LLM a cada conversa.

---

Em resumo: tecnicamente interessante, arquitetura genuína, benchmarks contestáveis, polêmica em torno da autoria — mas o código funciona e a ideia é sólida.
