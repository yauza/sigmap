FROM gcc:9

WORKDIR /app

COPY . .

RUN apt-get update && apt-get install vim -y

RUN make

CMD ["./sigmap"]