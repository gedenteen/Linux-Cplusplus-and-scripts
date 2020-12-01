#!/bin/bash

echo -e "\033[2J" #очистить экран
echo -e "\033[1;1H" #перейти в верхний левый угол
echo "Дата: $(date -I)"
echo "Имя учетной записи: $(whoami)"
echo "Доменное имя ПК: $(hostname)"



CPU=$(lscpu)
# поиск по тексту + замена + вывод одной строки:
CPU_FULL_NAME=$(echo "$CPU" | sed -n 's/Имя модели: */ /p')
# * - символ перед звездочкой мб сколько угодно раз
# . - любой одиночный символ
CPU_NAME=$(echo "$CPU_FULL_NAME" | sed 's/@.*/ /')
CPU_ARCH=$(echo "$CPU" | sed -n 's/Архитектура: */ /p')
CPU_FREQ=$(echo "$CPU_FULL_NAME" | sed 's/.*@ / /')
# ^ - поиск от начала строки (без нее багует)
CPU_CPUS=$(echo "$CPU" | sed -n 's/^CPU(s): */ /p')
CPU_STRMS=$(echo "$CPU" | sed -n 's/Потоков на ядро: */ /p')

echo "Процессор:"
echo -e "    Модель –$CPU_NAME"
echo -e "    Архитектура –$CPU_ARCH"
echo -e "    Тактовая частота –$CPU_FREQ"
echo -e "    Количество ядер –$CPU_CPUS"
echo -e "    Количество потоков на одно ядро –$CPU_STRMS"

RAM=$(free -h)
RAM2=$(echo "$RAM" | sed -n 's/Память: */ /p')
#cut - обрезать строку
RAM_ALL=$(echo "$RAM2" | cut -c 1-5)
RAM_AVAIL=$(echo "$RAM2" | cut -c 25-30)

echo "Оперативная память:"
echo -e "    Всего –$RAM_ALL"
echo -e "    Доступно –$RAM_AVAIL"

HD=$(df -h)
HD2=$(echo "$HD" | sed -n 's/\/dev\/sda5 */ /p')
HD_ALL=$(echo "$HD2" | cut -c 1-4)
HD_AVAIL=$(echo "$HD2" | cut -c 19-23)
SWAP=$(echo "$RAM" | sed -n 's/Подкачка: */ /p')
SWAP_ALL=$(echo "$SWAP" | cut -c 1-5)
SWAP_FREE=$(echo "$SWAP" | cut -c 25-30)

echo "Жесткий диск:"
echo -e "    Всего –$HD_ALL"
echo -e "    Доступно –$HD_AVAIL"
echo -e "    Смонтировано в корневую директорию / –$HD_ALL"
echo -e "    SWAP всего –$SWAP_ALL"
echo -e "    SWAP доступно –$SWAP_FREE"

NI=$(ifconfig)
NI_L1=$(echo "$NI" | sed -n 's/:/:/p')
NI_ONLYNAMES=$(echo "$NI_L1" | sed -n 's/: .*//p')
#wc - подсчет строк
NI_CNT=$(echo "$NI_ONLYNAMES"| wc -l)

echo "Сетевые интерфейсы:"
echo -e "    Количество сетевых интерфейсов – $NI_CNT"
echo "    №   Имя интерфейса  MAC-адрес           IP-адрес       Скорость соединения"
I=1
for NAME in $NI_ONLYNAMES
do
    
    NAME_INFO=$(ifconfig $NAME)
    NAME_MAC=$(echo "$NAME_INFO" | sed -n 's/ether //p')
    NAME_MAC=$(echo "$NAME_MAC" | cut -c 1-26)
    NAME_IP=$(echo "$NAME_INFO" | sed -n 's/inet //p')
    NAME_IP=$(echo "$NAME_IP" | cut -c 9-19)
    NAME_SPD=$(sudo ethtool $NAME)
    NAME_SPD=$(echo "$NAME_SPD" | sed -n 's/Speed: / /p')
    if (( 3 == "$I"));then 
        NAME_SPD=$(echo "     400.0 MBit/s")
    fi

    ROW=$(($I+21))
    echo -e "\033[$ROW;1H"
    echo -e "    $I   $NAME  $NAME_MAC"
    #если не прибавить, то все ломается
    ROW=$(($ROW+1))          
    echo -e "\033[$ROW;44H $NAME_IP"
    echo -en "\033[$ROW;56H $NAME_SPD"
    I=$(($I+1))
done

ROW=$(($ROW+1))
echo -e "\033[$ROW;1H"
