namespace Domain.Configuration;

public class ServiceSettings
{
    public DatabaseSettingsModel? DatabaseSettings { get; set; }
    
    public class DatabaseSettingsModel
    {
        public string? ConnectionString { get; set; }
    }
}