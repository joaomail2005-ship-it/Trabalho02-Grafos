$exe = ".\build\pcstp_solver.exe"
$pastaBase = ".\instances"
$subpastas = @("PCSPG-JMP", "PCSPG-CRR") 
$qtdPorPasta = 15
$iteracoes = "500"
$tamanhoBloco = "50"

$outputCsv = "tabela_final.csv"

# Cria o cabeçalho no formato esperado (usando ponto e vírgula para o Excel no Brasil ler mais fácil, ou vírgula)
# Deixamos as colunas best e Literatura vazias para você preencher depois se quiser.
"Instâncias;best;Literatura;Guloso;Randomizado alfa=0,2;Randomizado alfa=0,3;Randomizado alfa=0,4;Reativo" | Out-File -FilePath $outputCsv -Encoding utf8

function Get-Cost {
    param([array]$OutputLines)
    foreach ($line in $OutputLines) {
        if ($line -match "Melhor Custo Encontrado:\s*([\d\.]+)") {
            # Troca ponto por vírgula para ficar no padrão numérico do Excel PT-BR
            return $matches[1] -replace '\.', ','
        }
    }
    return ""
}

Write-Host "Iniciando a bateria completa de testes para gerar a planilha..." -ForegroundColor Green
Write-Host "Isso pode demorar um pouco, pois vai rodar todos os algoritmos para cada instancia!" -ForegroundColor DarkGray

foreach ($pasta in $subpastas) {
    $caminhoBusca = Join-Path $pastaBase $pasta
    
    if (Test-Path $caminhoBusca) {
        $instancias = @(Get-ChildItem -Path $caminhoBusca -Filter "*.stp" -File) | Select-Object -First $qtdPorPasta
        
        foreach ($arq in $instancias) {
            $caminhoStp = $arq.FullName
            $nomeInstancia = $arq.Name
            Write-Host "`nTestando: $nomeInstancia" -ForegroundColor Yellow
            
            # 1. Guloso
            Write-Host "  -> Guloso..." -ForegroundColor DarkGray -NoNewline
            $outGuloso = & $exe $caminhoStp "guloso"
            $custoGuloso = Get-Cost $outGuloso
            Write-Host " ($custoGuloso)" -ForegroundColor Gray
            
            # 2. Randomizado 0.2
            Write-Host "  -> Randomizado 0.2..." -ForegroundColor DarkGray -NoNewline
            $outRand2 = & $exe $caminhoStp "randomizado" "0.2" $iteracoes
            $custoRand2 = Get-Cost $outRand2
            Write-Host " ($custoRand2)" -ForegroundColor Gray

            # 3. Randomizado 0.3
            Write-Host "  -> Randomizado 0.3..." -ForegroundColor DarkGray -NoNewline
            $outRand3 = & $exe $caminhoStp "randomizado" "0.3" $iteracoes
            $custoRand3 = Get-Cost $outRand3
            Write-Host " ($custoRand3)" -ForegroundColor Gray

            # 4. Randomizado 0.4
            Write-Host "  -> Randomizado 0.4..." -ForegroundColor DarkGray -NoNewline
            $outRand4 = & $exe $caminhoStp "randomizado" "0.4" $iteracoes
            $custoRand4 = Get-Cost $outRand4
            Write-Host " ($custoRand4)" -ForegroundColor Gray

            # 5. Reativo
            Write-Host "  -> Reativo..." -ForegroundColor DarkGray -NoNewline
            $outReativo = & $exe $caminhoStp "reativo" $iteracoes $tamanhoBloco
            $custoReativo = Get-Cost $outReativo
            Write-Host " ($custoReativo)" -ForegroundColor Gray
            
            # Salva na linha do CSV separando por ponto e vírgula
            $linha = "$nomeInstancia;;;$custoGuloso;$custoRand2;$custoRand3;$custoRand4;$custoReativo"
            $linha | Out-File -FilePath $outputCsv -Append -Encoding utf8
        }
    }
}

Write-Host "`nPlanilha gerada com sucesso em: $outputCsv" -ForegroundColor Green
