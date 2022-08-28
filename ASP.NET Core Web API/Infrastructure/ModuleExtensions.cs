using System;
using Domain;
using Domain.Configuration;
using Domain.RepositoryInterfaces;
using Infrastructure.Persistence.SqLite.Common;
using Infrastructure.Persistence.SqLite.Repositories;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;

namespace Infrastructure;

public static class ModuleExtensions
{
    public static void AddPersistenceModule(this IServiceCollection services, IConfiguration configuration)
    {
        var serviceSettings = configuration.GetSection("ServiceSettings").Get<ServiceSettings>();
        var connectionString = serviceSettings.DatabaseSettings?.ConnectionString;

        if (!string.IsNullOrWhiteSpace(connectionString))
        {
            services.AddScoped<IPairRepository, PairRepository>(_ => new PairRepository(connectionString));
            SqLiteHelper.PrepareDatabase();
        }
        else
        {
            Console.WriteLine(StringResources.Connection_String_Is_Empty);
            Console.WriteLine(StringResources.Exiting_Application);
            Environment.Exit(1);
        }
    }
}