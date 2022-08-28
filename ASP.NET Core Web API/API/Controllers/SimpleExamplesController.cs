using Microsoft.AspNetCore.Mvc;

namespace API.Controllers;

[ApiController]
public class SimpleExamplesController : ControllerBase
{
    [HttpGet("/api/hello-world")]
    public IActionResult SayHelloWorld()
    {
        return Ok("Hello World");
    }

    [HttpGet("/api/print/{input}")]
    public IActionResult Echo(string input)
    {
        return Ok($"{input}");
    }
}