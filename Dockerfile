FROM gcc:latest

WORKDIR /app

COPY . .

RUN make

CMD ["./sigmap"]