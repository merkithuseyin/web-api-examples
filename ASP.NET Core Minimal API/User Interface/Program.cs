using Business_Logic;
using Microsoft.AspNetCore.Builder;
using Microsoft.Extensions.DependencyInjection;

namespace Minimal_API
{
    public static class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);
            builder.Services.AddHealthChecks();
            builder.Services.AddEndpointsApiExplorer();
            builder.Services.AddSwaggerGen();

            var app = builder.Build();
            app.MapHealthChecks("/healthz");
            app.UseSwagger();
            app.UseSwaggerUI(options =>
            {
                options.SwaggerEndpoint("/swagger/v1/swagger.json", "v1");
                options.RoutePrefix = string.Empty;
            });

            Data_Access.SqLiteHelper.PrepareDatabase();
            AddEndpoints(app);
            
            app.Run();
        }

        private static void AddEndpoints(WebApplication webApplication)
        { 
            // simple examples
            webApplication.MapGet("api/hello-world", () => "Hello World");
            webApplication.MapGet("api/print/{input}", (string input) => $"{input}");
            
            // adding "pair" related endpoints
            IPairBLL pairBLL = new PairBLL();
            
            webApplication.MapGet("api/pairs/", async () => 
                await pairBLL.GetAllAsync());
            
            webApplication.MapGet("api/pairs/{id:int}", async (int id) => 
                await pairBLL.GetAsync(id));

            webApplication.MapPost("api/pairs/", async (PairRequest pair) =>
                await pairBLL.SaveAsync(new Data_Access.Models.Pair {Name = pair.Name, Value = pair.Value}));

            webApplication.MapMethods("api/pairs/{id:int}", new[] {"PATCH"}, async (PairRequest pair, int id) =>
                await pairBLL.UpdateAsync(id, new Data_Access.Models.Pair {Name = pair.Name, Value = pair.Value}));
            
            webApplication.MapDelete("api/pairs/{id:int}", async (int id) => 
                await pairBLL.DeleteAsync(id));
        }

        record PairRequest(string Name, string Value);
    }
}