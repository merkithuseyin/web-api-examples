FROM mcr.microsoft.com/dotnet/aspnet:6.0 AS runtime
FROM mcr.microsoft.com/dotnet/sdk:6.0 AS sdk

FROM sdk AS build
WORKDIR "/src/ASP Net Core Minimal API"
COPY . .
RUN dotnet restore "User Interface/User Interface.csproj"
RUN dotnet build "User Interface/User Interface.csproj" -c Release -o /app/build
RUN dotnet publish "User Interface/User Interface.csproj" -c Release -o /app/publish

FROM runtime
WORKDIR /app
COPY --from=build /app/publish .
RUN apt-get update
RUN apt-get install -y curl
ENV port=5000
ENV ASPNETCORE_URLS=http://+:$port
HEALTHCHECK CMD curl --fail http://localhost:$port/healthz || exit
ENTRYPOINT ["dotnet", "User Interface.dll"]